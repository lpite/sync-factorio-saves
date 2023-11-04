#include <iostream>
#include <string>
#include <array>
#include <chrono>
#include <thread>
#include <sstream>

std::string exec(const char* cmd) {

    std::cout << "executed = " << cmd << '\n';
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    if (_pclose(pipe.get()) != 0) {
        throw std::runtime_error("Command execution failed!");
    }
    return result;
}
struct CONFIG {
    std::string userFolder;
    std::string userName;
    std::string gitAdd;
    std::string gitCommit;
    std::string gitPush;
    std::string gitPull;
};

std::string getUserName() {
    std::string output = exec("whoami");

    size_t slashPos = output.find("\\");
    output.erase(0, slashPos + 1);

    //removing new line symbol on the end of user name
    output.erase(output.size() - 1, output.size());

    std::cout << "your username is = " << output << '\n';
    return output;
}

int countLines(std::string str) {
    return std::count(str.begin(), str.end(), '\n') - 2;
}

void pushToGit(CONFIG& config) {
    
    system(config.gitAdd.c_str());
    system(config.gitCommit.c_str());
    system(config.gitPush.c_str());
}

void main() {
    CONFIG config;

    config.userName = getUserName();

    config.userFolder = "C:\\Users\\" + config.userName + "\\AppData\\Roaming\\Factorio\\saves";
    std::cout << "saves folder is = " << config.userFolder << '\n';

    config.gitPull = "cd " + config.userFolder + " && git pull origin main";
    config.gitAdd = "cd " + config.userFolder + " && git add .";
    config.gitCommit = "cd " + config.userFolder + " && git commit -m \"changed\"";
    config.gitPush = "cd " + config.userFolder + " && git push";
    system(config.gitPull.c_str());
    std::string prev = "";

    while (true) {
        std::string output = exec(("cd " + config.userFolder + " && dir").c_str());
        int n = countLines(output);

        std::istringstream ss(output);
        std::string line;

        int i = 0;

        while (std::getline(ss, line))
        {
            if (i == n)
            {

                if (prev != line && prev.size()) {
                    pushToGit(config);
                }

                prev = line;

            }
            i++;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

}
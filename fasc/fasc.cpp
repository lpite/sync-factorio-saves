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
};

//
//
//int main() {
//
//    CONFIG config = {
//        "&& git add .",
//        "&& git commit -m \"test\\",
//        "&& git push",
//        1
//    };
//    std::string prevStr = "";
//    std::string userName = "";
//    std::cout << "enter ur username\n";
//    std::cin >> userName;
//    //std::cout << "enter ur lang\n 1:engl \n 2:ru \n 3:uk \n";
//    //std::cin >> config.lang;
//    const std::string SAVES_FOLDER = "C:\\Users\\" + userName + "\\AppData\\Roaming\\Factorio\\saves";
//    std::string command = "cd " + SAVES_FOLDER + "&& dir";
//
//    try {
//        while (true)
//        {
//            std::string output = exec(command.c_str());
//            std::string s = output;
//
//            std::string filesStr = "File(s)";
//            if (config.lang == 2)
//            {
//                filesStr = "файлов";
//            }
//
//            size_t pos = s.find(filesStr);
//            std::cout << pos;
//            s.erase(0, pos + filesStr.length());
//            std::string bytesStr = "bytes";
//            if (config.lang == 2)
//            {
//                bytesStr = "байт";
//            }
//            s.erase(s.find(bytesStr),s.length()- s.find(bytesStr));
//
//
//            std::string del = ",";
//            if (config.lang == 2)
//            {
//                del = " ";
//            }
//
//            while (s.find(del) != std::string::npos)
//            {
//             s.erase(s.find(del), del.length());
//            }
//            if (prevStr == "")
//            {
//                prevStr = s;
//            }
//            else {
//                if (prevStr != s)
//                {
//                    prevStr = s;
//                    system(("cd " + SAVES_FOLDER + config.gitAdd).c_str());
//                    system(("cd " + SAVES_FOLDER + config.gitCommit).c_str());
//                    system(("cd " + SAVES_FOLDER + config.gitPush).c_str());
//                }
//            }
//            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//        }
//
//    }
//    catch (const std::exception& e) {
//        std::cerr << "Error: " << e.what() << std::endl;
//    }
//    return 0;
//}

std::string getUserName() {
    std::string output = exec("whoami");
    
    size_t slashPos = output.find("\\");
    output.erase(0, slashPos + 1);

    //removing new line symbol on the end of user name
    output.erase(output.size()-1, output.size());

    std::cout << "your username is = " << output << '\n';
    return output;
}

int countLines(std::string str) {
    return std::count(str.begin(), str.end(), '\n') - 2;
}

void pushToGit(CONFIG &config) {
    system(config.gitAdd.c_str());
    system(config.gitCommit.c_str());
    system(config.gitPush.c_str());
}

void main() {
    CONFIG config;

    config.userName = getUserName();
    
    config.userFolder = "C:\\Users\\" + config.userName +"\\AppData\\Roaming\\Factorio\\saves";
    std::cout << "saves folder is = " << config.userFolder << '\n';

    config.gitAdd = "cd " + config.userFolder + " && git add .";
    config.gitCommit = "cd " + config.userFolder + " && git commit -m \"changed\"";
    config.gitPush = "cd " + config.userFolder + " && git push";
    std::cout << "git comm = " << config.gitPush << '\n';

    std::string prev = "";
    
    while (true) {
        std::string output = exec(("cd " + config.userFolder +" && dir").c_str());
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

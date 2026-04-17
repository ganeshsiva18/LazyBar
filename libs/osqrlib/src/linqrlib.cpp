#include <cstdlib>
#include <string>

void openurl(const std::string& url) {
    std::string command = "xdg-open " + url;
    std::system(command.c_str());
}
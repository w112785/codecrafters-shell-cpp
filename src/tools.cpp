#include "tools.hpp"
#include "Modes.hpp"

std::vector<std::string> SplitString(const std::string& str, char delimiter, bool tokenize) {
    std::istringstream iss(str);
    std::vector<std::string> tokens;
    std::string token;
    Modes mode = Modes::NormalMode;
    bool escaped = false;
    if (tokenize) {
        for (size_t i = 0; i < str.length(); i++) {
            char c = str[i];
            bool modechanged = false;

            switch (mode) {
            case Modes::NormalMode:
                if (escaped) { // previous char was '\'
                    token += c;
                    escaped = false;
                    continue;
                }
                if (c == '\'') {
                    mode = Modes::SingleQuote;
                    modechanged = true;
                } else if (c == '"') {
                    mode = Modes::DoubleQuote;
                    modechanged = true;
                } else if (c == '\\') {
                    escaped = true;
                    continue;
                }
                break;

            case Modes::SingleQuote:
                if (c == '\'') {
                    mode = Modes::NormalMode;
                    modechanged = true;
                }
                break;

            case Modes::DoubleQuote:
                if (escaped) {
                    if (c == '\\' || c == '"') {
                        token += c;
                        escaped = false;
                    } else {
                        token += std::format("\\{}", c);
                        escaped = false;
                    }
                    continue;
                }

                if (c == '"' && !escaped) {
                    mode = Modes::NormalMode;
                    modechanged = true;
                } else if (c == '\\') {
                    escaped = true;
                    continue;
                }

                break;
            }
            if (escaped && std::isspace(c)) {
                token += c;
                escaped = false;
            }
            if (c == delimiter && mode == Modes::NormalMode) {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token.clear();
                }
            } else if (!modechanged) {
                token += c;
            }
        }
        if (!token.empty()) {
            tokens.push_back(token);
            token.clear();

        }
    } else {
        while (std::getline(iss, token, delimiter)) {
            tokens.push_back(token);
            token.clear();
        }
    }
    return tokens;
}


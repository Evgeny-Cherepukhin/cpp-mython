// Итоговый проект 15 спринт. Черепухин Евгений Сергеевич.
#pragma once

#include <iosfwd>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <variant>

namespace parse {

    namespace token_type {
        struct Number {  // Лексема «число»
            int value;   // число
        };

        struct Id {             // Лексема «идентификатор»
            std::string value;  // Имя идентификатора
        };

        struct Char {    // Лексема «символ»
            char value;  // код символа
        };

        struct String {  // Лексема «строковая константа»
            std::string value;
        };

        struct Class {
        };    // Лексема «class»
        struct Return {
        };   // Лексема «return»
        struct If {
        };       // Лексема «if»
        struct Else {
        };     // Лексема «else»
        struct Def {
        };      // Лексема «def»
        struct Newline {
        };  // Лексема «конец строки»
        struct Print {
        };    // Лексема «print»
        struct Indent {
        };  // Лексема «увеличение отступа», соответствует двум пробелам
        struct Dedent {
        };  // Лексема «уменьшение отступа»
        struct Eof {
        };     // Лексема «конец файла»
        struct And {
        };     // Лексема «and»
        struct Or {
        };      // Лексема «or»
        struct Not {
        };     // Лексема «not»
        struct Eq {
        };      // Лексема «==»
        struct NotEq {
        };   // Лексема «!=»
        struct LessOrEq {
        };     // Лексема «<=»
        struct GreaterOrEq {
        };  // Лексема «>=»
        struct None {
        };         // Лексема «None»
        struct True {
        };         // Лексема «True»
        struct False {
        };        // Лексема «False»
    }  // namespace token_type

    using TokenBase
    = std::variant<token_type::Number, token_type::Id, token_type::Char, token_type::String,
            token_type::Class, token_type::Return, token_type::If, token_type::Else,
            token_type::Def, token_type::Newline, token_type::Print, token_type::Indent,
            token_type::Dedent, token_type::And, token_type::Or, token_type::Not,
            token_type::Eq, token_type::NotEq, token_type::LessOrEq, token_type::GreaterOrEq,
            token_type::None, token_type::True, token_type::False, token_type::Eof>;

    struct Token : TokenBase {
        using TokenBase::TokenBase;

        template<typename T>
        [[nodiscard]] bool Is() const {
            return std::holds_alternative<T>(*this);
        }

        template<typename T>
        [[nodiscard]] const T &As() const {
            return std::get<T>(*this);
        }

        template<typename T>
        [[nodiscard]] const T *TryAs() const {
            return std::get_if<T>(this);
        }
    };

    bool operator==(const Token &lhs, const Token &rhs);

    bool operator!=(const Token &lhs, const Token &rhs);

    std::ostream &operator<<(std::ostream &os, const Token &rhs);

    class LexerError : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    class Lexer {
    public:
        explicit Lexer(std::istream &input);

        // Возвращает ссылку на текущий токен или token_type::Eof, если поток токенов закончился
        [[nodiscard]] const Token &CurrentToken() const;

        // Возвращает следующий токен, либо token_type::Eof, если поток токенов закончился
        Token NextToken();

        // Если текущий токен имеет тип T, метод возвращает ссылку на него.
        // В противном случае метод выбрасывает исключение LexerError
        template<typename T>
        const T &Expect() const {
            using namespace std::literals;
            if (CurrentToken().Is<T>()) return CurrentToken().As<T>();
            throw LexerError("Not expected current token type"s);
        }

        // Метод проверяет, что текущий токен имеет тип T, а сам токен содержит значение value.
        // В противном случае метод выбрасывает исключение LexerError
        template<typename T, typename U>
        void Expect(const U &value) const {
            using namespace std::literals;
            if (!CurrentToken().Is<T>() || CurrentToken().As<T>().value != value) {
                throw LexerError("Not expected current token type or value"s);
            }
        }

        // Если следующий токен имеет тип T, метод возвращает ссылку на него.
        // В противном случае метод выбрасывает исключение LexerError
        template<typename T>
        const T &ExpectNext() {
            using namespace std::literals;
            Token next = NextToken();
            if (!next.Is<T>()) {
                throw LexerError("Not expected next token type"s);
            }
            return CurrentToken().As<T>();
        }

        // Метод проверяет, что следующий токен имеет тип T, а сам токен содержит значение value.
        // В противном случае метод выбрасывает исключение LexerError
        template<typename T, typename U>
        void ExpectNext(const U &value) {
            using namespace std::literals;
            Token next = NextToken();
            if (!next.Is<T>() || next.As<T>().value != value) {
                throw LexerError("Not expected next token type or value"s);
            }
        }

    private:
        struct TextLine {
            static Lexer::TextLine ReadLine(std::istream &in);

            static int SkipSpaces(std::istream &in);

            static void SkipComment(std::istream &in);

            void ReadString(std::istream &in, int begin_quote);

            void ReadNumber(std::istream &in, int first_dig);

            void ReadIdentifier(std::istream &in, int first_sym);

            void ReadComparison(std::istream &in, int ch);

            bool IsEmpty() const;

            bool IsEofOnly() const;

            int indent_ = 0;
            std::vector<Token> tokens_;
        };

        std::istream &in_;
        int current_indent_ = 0;
        int curr_pos_ = -1;
        std::vector<Token> tokens_;
    };

}  // namespace parse
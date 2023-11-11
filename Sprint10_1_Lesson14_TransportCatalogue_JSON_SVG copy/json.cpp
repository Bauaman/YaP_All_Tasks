#include "json.h"

namespace json {

    Node LoadNode(std::istream& input);

    std::string LoadAlpha(std::istream& input) {
        std::string s;
        while (std::isalpha(input.peek())) {
            s.push_back(static_cast<char>(input.get()));
        }
        return s;
    }

    Node LoadNull(std::istream& input) {
        auto word = LoadAlpha(input);
        if (word == "null") {
            return Node(nullptr);
        } else {
            throw ParsingError("Null parsing error");
        }
    }

    Node LoadBool(std::istream& input) {
        auto word = LoadAlpha(input);
        if (word == "true") {
            return Node(true);
        } else if (word == "false") {
            return Node(false);
        } else {
            throw ParsingError("Bool parsing error");
        }
    }

    Node LoadNum(std::istream& input) {
        using namespace std::literals;

        std::string parsed_num;

        // Считывает в parsed_num очередной символ из input
        auto read_char = [&parsed_num, &input] {
            parsed_num += static_cast<char>(input.get());
            if (!input) {
                throw ParsingError("Failed to read number from stream");
            }
        };

        // Считывает одну или более цифр в parsed_num из input
        auto read_digits = [&input, read_char] {
            if (!std::isdigit(input.peek())) {
                throw ParsingError("A digit is expected");
            }
            while (std::isdigit(input.peek())) {
                read_char();
            }
        };

        if (input.peek() == '-') {
            read_char();
        }
        // Парсим целую часть числа
        if (input.peek() == '0') {
            read_char();
            // После 0 в JSON не могут идти другие цифры
        } else {
            read_digits();
        }

        bool is_int = true;
        // Парсим дробную часть числа
        if (input.peek() == '.') {
            read_char();
            read_digits();
            is_int = false;
        }

        // Парсим экспоненциальную часть числа
        if (int ch = input.peek(); ch == 'e' || ch == 'E') {
            read_char();
            if (ch = input.peek(); ch == '+' || ch == '-') {
                read_char();
            }
            read_digits();
            is_int = false;
        }

        try {
            if (is_int) {
                // Сначала пробуем преобразовать строку в int
                try {
                    return std::stoi(parsed_num);
                } catch (...) {
                    // В случае неудачи, например, при переполнении,
                    // код ниже попробует преобразовать строку в double
                }
            }
            return std::stod(parsed_num);
        } catch (...) {
            throw ParsingError("Failed to convert "s + parsed_num + " to number"s);
        }
    }

    Node LoadString(std::istream& input) {
    using namespace std::literals;
    
    auto it = std::istreambuf_iterator<char>(input);
    auto end = std::istreambuf_iterator<char>();
    std::string s;
    while (true) {
        if (it == end) {
            // Поток закончился до того, как встретили закрывающую кавычку?
            throw ParsingError("String parsing error");
        }
        const char ch = *it;
        if (ch == '"') {
            // Встретили закрывающую кавычку
            ++it;
            break;
        } else if (ch == '\\') {
            // Встретили начало escape-последовательности
            ++it;
            if (it == end) {
                // Поток завершился сразу после символа обратной косой черты
                throw ParsingError("String parsing error");
            }
            const char escaped_char = *(it);
            // Обрабатываем одну из последовательностей: \\, \n, \t, \r, \"
            switch (escaped_char) {
                case 'n':
                    s.push_back('\n');
                    break;
                case 't':
                    s.push_back('\t');
                    break;
                case 'r':
                    s.push_back('\r');
                    break;
                case '"':
                    s.push_back('"');
                    break;
                case '\\':
                    s.push_back('\\');
                    break;
                default:
                    // Встретили неизвестную escape-последовательность
                    throw ParsingError("Unrecognized escape sequence \\"s + escaped_char);
            }
        } else if (ch == '\n' || ch == '\r') {
            // Строковый литерал внутри- JSON не может прерываться символами \r или \n
            throw ParsingError("Unexpected end of line"s);
        } else {
            // Просто считываем очередной символ и помещаем его в результирующую строку
            s.push_back(ch);
        }
        ++it;
    }
    return Node(std::move(s));
}

    Node LoadArray(std::istream& input) {
        Array result;
        for (char c; input >> c && c!= ']';) {
            if (c != ',') {
                input.putback(c);
            }
            result.push_back(LoadNode(input));
        }
        if (input.peek() == -1) {
            throw ParsingError("Map parsing error");
        }
        return Node(std::move(result));    
    }

    Node LoadMap(std::istream& input) {
        Dict result;
        for (char c; input >> c && c != '}';) {
            if (c== '"') {
                std::string key = LoadString(input).AsString();
                if (input >> c && c == ':') {
                    if (result.find(key) != result.end()) {
                        throw ParsingError("Duplicate key" + key);
                    }
                    result.emplace(std::move(key), LoadNode(input));
                } else {
                    throw ParsingError("Error parsing map - no \":\" found");
                }
            } else if (c != ',') {
                throw ParsingError("Error parsing map - no \",\" found");
            }
        }
        return Node(std::move(result));
    }

    Node LoadNode(std::istream& input) {
        char c;
        input >> c;
        if (c == '{') {
            return LoadMap(input);
        } else if (c == '[') {
            return LoadArray(input);
        } else if (c == '"') {
            return LoadString(input);
        } else if (c == 'n') {
            input.putback(c);
            return LoadNull(input);
        } else if (c == 'f' || c == 't') {
            input.putback(c);
            return LoadBool(input);
        } else {
            input.putback(c);
            return LoadNum(input);
        }
    }

    Document Load(std::istream& input) {
        return Document{LoadNode(input)};
    }

    //Конструкторы
    Node::Node(std::nullptr_t) :
        value_(nullptr) 
    {}
    Node::Node(std::string val) :
        value_(std::move(val))
    {}
    Node::Node(double val) :
        value_(val)
    {}
    Node::Node(int val) :
        value_(val)
    {}
    Node::Node(bool val) :
        value_(val)
    {}
    Node::Node(Array array) :
        value_(std::move(array))
    {}
    Node::Node(Dict map) :
        value_(std::move(map))
    {}


    //Проверка типа
    bool Node::IsNull() const {
        return (std::holds_alternative<std::nullptr_t>(value_));
    }
    bool Node::IsDict() const {
        return (std::holds_alternative<Dict>(value_));
    }
    bool Node::IsString() const {
        return (std::holds_alternative<std::string>(value_));
    }
    bool Node::IsDouble() const {
        return (std::holds_alternative<int>(value_) || std::holds_alternative<double>(value_));
    }
    bool Node::IsArray() const {
        return (std::holds_alternative<Array>(value_));
    }
    bool Node::IsBool() const {
        return (std::holds_alternative<bool>(value_));
    }
    bool Node::IsInt() const {
        return (std::holds_alternative<int>(value_));
    }

    //As*
    const std::string& Node::AsString() const {
        if (!IsString()) {
            throw std::logic_error("Not a string");
        }
        return std::get<std::string>(value_);
    }
    const Dict& Node::AsDict() const {
        if(!IsDict()) {
            throw std::logic_error("Not a map");
        }
        return std::get<Dict>(value_);
    }
    double Node::AsDouble() const {
        if (!IsDouble()) {
            throw std::logic_error("Not a double");
        }
        return std::get<double>(value_);
    }
    const Array& Node::AsArray() const {
        if (!IsArray()) {
            throw std::logic_error("Not an Array");
        }
        return std::get<Array>(value_);
    }
    bool Node::AsBool() const {
        if (!IsBool()) {
            throw std::logic_error("Not boolean");
        }
        return std::get<bool>(value_);
    }
    int Node::AsInt() const {
        if (!IsInt()) {
            throw std::logic_error("Not an integer");
        }
        return std::get<int>(value_);
    }


    //
    const Node::Value& Node::GetValue() const {
        return value_;
    }
    bool Node::operator==(const Node& rhs) const {
        return value_ == rhs.GetValue();
    }
    bool Node::operator!=(const Node& rhs) const {
        return !(value_ == rhs.GetValue());
    }
    Document::Document(Node root)
        : root_(std::move(root)) {
    }
    const Node& Document::GetRoot() const {
        return root_;
    }
    bool Document::operator==(const Document& rhs) const {
        return root_ == rhs.GetRoot();
    }
    bool Document::operator!=(const Document& rhs) const {
        return !(root_ == rhs.GetRoot());
    }

}//namespace json
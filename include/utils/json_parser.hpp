#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include <stdexcept>
#include <cctype>

class Json {
public:
    using object_t = std::unordered_map<std::string, Json>;
    using array_t  = std::vector<Json>;
    using value_t  = std::variant<std::nullptr_t, bool, double, std::string, object_t, array_t>;

    Json() : value(nullptr) {}
    Json(bool b) : value(b) {}
    Json(double n) : value(n) {}
    Json(const std::string& s) : value(s) {}
    Json(const object_t& o) : value(o) {}
    Json(const array_t& a) : value(a) {}

    bool is_object() const { return std::holds_alternative<object_t>(value); }
    bool is_array()  const { return std::holds_alternative<array_t>(value); }
    bool is_string() const { return std::holds_alternative<std::string>(value); }
    bool is_bool()   const { return std::holds_alternative<bool>(value); }
    bool is_number() const { return std::holds_alternative<double>(value); }
    bool is_null()   const { return std::holds_alternative<std::nullptr_t>(value); }

    const object_t& as_object() const { return std::get<object_t>(value); }
    const array_t&  as_array()  const { return std::get<array_t>(value); }
    const std::string& as_string() const { return std::get<std::string>(value); }
    double as_number() const { return std::get<double>(value); }
    bool as_bool() const { return std::get<bool>(value); }

    static Json parse(const std::string& text) {
        size_t pos = 0;
        return parse_value(text, pos);
    }

private:
    value_t value;

    static void skip_ws(const std::string& s, size_t& pos) {
        while (pos < s.size() && std::isspace(s[pos])) pos++;
    }

    static Json parse_value(const std::string& s, size_t& pos) {
        skip_ws(s, pos);
        if (pos >= s.size()) throw std::runtime_error("Unexpected end of input");

        char c = s[pos];
        if (c == '"') return parse_string(s, pos);
        if (c == '{') return parse_object(s, pos);
        if (c == '[') return parse_array(s, pos);
        if (c == 't' || c == 'f') return parse_bool(s, pos);
        if (c == 'n') return parse_null(s, pos);
        if (c == '-' || std::isdigit(c)) return parse_number(s, pos);

        throw std::runtime_error("Invalid JSON value");
    }

    static Json parse_string(const std::string& s, size_t& pos) {
        pos++; // skip opening quote
        std::string out;
        while (pos < s.size()) {
            char c = s[pos++];
            if (c == '"') break;
            if (c == '\\') {
                if (pos >= s.size()) throw std::runtime_error("Invalid escape");
                char esc = s[pos++];
                switch (esc) {
                    case 'n': out.push_back('\n'); break;
                    case 't': out.push_back('\t'); break;
                    case 'r': out.push_back('\r'); break;
                    case '"': case '\\': case '/': out.push_back(esc); break;
                    default: throw std::runtime_error("Unsupported escape");
                }
            } else {
                out.push_back(c);
            }
        }
        return Json(out);
    }

    static Json parse_number(const std::string& s, size_t& pos) {
        size_t start = pos;
        if (s[pos] == '-') pos++;
        while (pos < s.size() && std::isdigit(s[pos])) pos++;
        if (pos < s.size() && s[pos] == '.') {
            pos++;
            while (pos < s.size() && std::isdigit(s[pos])) pos++;
        }
        return Json(std::stod(s.substr(start, pos - start)));
    }

    static Json parse_bool(const std::string& s, size_t& pos) {
        if (s.compare(pos, 4, "true") == 0) { pos += 4; return Json(true); }
        if (s.compare(pos, 5, "false") == 0) { pos += 5; return Json(false); }
        throw std::runtime_error("Invalid boolean");
    }

    static Json parse_null(const std::string& s, size_t& pos) {
        if (s.compare(pos, 4, "null") == 0) { pos += 4; return Json(); }
        throw std::runtime_error("Invalid null");
    }

    static Json parse_array(const std::string& s, size_t& pos) {
        pos++; // skip '['
        array_t arr;
        skip_ws(s, pos);
        if (s[pos] == ']') { pos++; return Json(arr); }

        while (true) {
            arr.push_back(parse_value(s, pos));
            skip_ws(s, pos);
            if (s[pos] == ']') { pos++; break; }
            if (s[pos] != ',') throw std::runtime_error("Expected ','");
            pos++;
        }
        return Json(arr);
    }

    static Json parse_object(const std::string& s, size_t& pos) {
        pos++; // skip '{'
        object_t obj;
        skip_ws(s, pos);
        if (s[pos] == '}') { pos++; return Json(obj); }

        while (true) {
            skip_ws(s, pos);
            if (s[pos] != '"') throw std::runtime_error("Expected string key");
            std::string key = parse_string(s, pos).as_string();

            skip_ws(s, pos);
            if (s[pos] != ':') throw std::runtime_error("Expected ':'");
            pos++;

            Json val = parse_value(s, pos);
            obj.emplace(std::move(key), std::move(val));

            skip_ws(s, pos);
            if (s[pos] == '}') { pos++; break; }
            if (s[pos] != ',') throw std::runtime_error("Expected ','");
            pos++;
        }
        return Json(obj);
    }
};

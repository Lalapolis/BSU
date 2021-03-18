#include <iostream>

class String {
    char* string_;
    int size_;
    
  public:
    String() {
       size_ = 0;
       string_ = nullptr;
    }
    
    explicit String(const char* str) {
        size_ = strlen(str);
        string_ = new char[size_];
        for (int i = 0; i < size_; ++i) {
            string_[i] = str[i];
        }
    }

    ~String() {
        delete[] string_;
    }
    
    String(const String& str) {
        size_ = str.size_;
        string_ = new char[size_];
        for (int i = 0; i < size_; ++i) {
            string_[i] = str.string_[i];
        }
    }
    
    int GetSize() {
        return size_;
    }
    
    String& operator=(const String& other) {
        size_ = other.size_;
        delete[] string_;
        string_ = new char[size_];
        for (int i = 0; i < size_; ++i) {
            string_[i] = other.string_[i];
        }
        return *this;
    }
    
    friend std::ostream& operator<<(std::ostream& out, const String& str) {
        for (int i = 0; i < str.size_; ++i) {
            out << str.string_[i];
        }
        return out;
    }
    
    String& operator+=(const String& other) {
        char* copy = new char[size_ + other.size_];
        for (int i = 0; i < size_; ++i) {
            copy[i] = string_[i];
        }
        for (int i = 0; i < other.size_; ++i) {
            copy[i + size_] = other.string_[i];
        }
        
        delete[] string_;
        size_ = size_ + other.size_;
        string_ = copy;
        
        return *this;
    }
    
    String operator+(const String& other) {
        String result(*this);        
        result += other;                  
        return result;
    }
    
    
    friend String operator*(int num, const String& str) {
        String result;
        if (num > 0) {
            for (int i = 0; i < num; i++) {
                result += str;   
            }
        } else if (num < 0) {
            throw std::runtime_error("Can't be multiplied by a negative number");
        }
        return result;
    }
    
    String operator*(int num) {
        String result;
        if (num > 0) {
            for (int i = 0; i < num; i++) {
                result += *this;
            }
        } else if (num < 0) {
            throw std::runtime_error("Can't be multiplied by a negative number");
        }
        return result;
    }
    
    char& operator[](int index) {
        if (index < 0 || index >= size_) {
            throw std::runtime_error("Incorrect index");
        } else {
            return string_[index];
        }
    }
    
    const char& operator[](int index) const {
        if (index < 0 || index >= size_) {
            throw std::runtime_error("Incorrect index");
        } else {
            return string_[index];  
        }
    }
    
    String& operator+=(char c) {
        char s[2];
        s[0] = c;
        s[1] = '\0';
        
        String tmp(s);

        *this += tmp;
        return *this;
    }
                       
    friend String operator+(const String& string, char c)
    {
        String copy(string);
        copy += c;
        return copy;
    }
};


int main() {
    String string("abacaba");
    std::cout << string << std::endl;
    
    string = String("foo");
    string += 'd';
    std::cout << (string + 'f') << std::endl;
    
    std::cout << string << std::endl;
    std::cout << String("bar") << std::endl;

    String stringA = String("A");
    String stringB = String("B");
    stringA += stringB;
    
    std::cout << stringA << std::endl;        
    std::cout << stringA + stringB << std::endl;
    
    try {
        // "ab" * 3 = "ababab"
        // 3 * "ab" = "ababab"
        std::cout << stringA * 3 << std::endl;
        std::cout << 3 * (stringB + stringA) << std::endl;
        std::cout << stringA * 0 << std::endl;
        std::cout << stringA * (-1) << std::endl;
        std::cout << stringA * 2 << std::endl;
    } catch (const std::exception& error) {
        std::cout << error.what() << std::endl;
    }
    
    try {
        string[1] = 'a';
        throw "Exception";
        throw std::runtime_error("runtime error");
        throw 1; 
    } catch (const std::exception& error) {
        std::cout << error.what() << std::endl;
    } catch (const std::runtime_error& re) {
        std::cout << "ERROR" << std::endl;
    } catch (int error) {
        std::cout << "Error type: " << error << std::endl;
    }
    
    for (int i = 0; i < stringA.GetSize(); ++i) {
        std::cout << stringA[i] << std::endl;
    }
    return 0;
}
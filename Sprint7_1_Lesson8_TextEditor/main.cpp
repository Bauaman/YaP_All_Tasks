#include <iostream>
#include <list>
#include <string>

using namespace std;
using namespace std::string_literals;

class Editor {
public:
    Editor() :
        text_(),
        buffer_(),
        cursor_(text_.begin())
    {
    }
    
    // сдвинуть курсор влево
    void Left();
    // сдвинуть курсор вправо 
    void Right();
    // вставить символ token
    void Insert(char token);
    // вырезать не более tokens символов, начиная с текущей позиции курсора
    void Cut(size_t tokens = 1);
    // cкопировать не более tokens символов, начиная с текущей позиции курсора
    void Copy(size_t tokens = 1);
    // вставить содержимое буфера в текущую позицию курсора
    void Paste();
    // получить текущее содержимое текстового редактора
    string GetText() const;
    string GetBuffer() const;

private:
    list<char> text_;
    list<char> buffer_;
    list<char>::iterator cursor_;
};

void Editor::Insert(char token) {
    text_.insert(cursor_,token);
}

void Editor::Paste() {
    text_.insert(cursor_, buffer_.begin(), buffer_.end());
}

void Editor::Cut(size_t tokens) {
    if (tokens > static_cast<size_t>(distance(cursor_,text_.end()))) {
        tokens = static_cast<size_t>(distance(cursor_,text_.end()));
    }
    auto end_of_range = next(cursor_,tokens);
    buffer_ = {cursor_,end_of_range};
    cursor_ = text_.erase(cursor_,end_of_range);
}

void Editor::Copy(size_t tokens) {
    if (tokens > static_cast<size_t>(distance(cursor_,text_.end()))) {
        tokens = static_cast<size_t>(distance(cursor_,text_.end()));
    }
    auto end_of_range = next(cursor_,tokens);
    buffer_ = {cursor_,end_of_range};
}

void Editor::Left() {
    if (cursor_ == text_.begin() || text_.empty()) {
        return;
    }
    --cursor_;
}

void Editor::Right() {
    if (cursor_ == text_.end() || text_.empty()) {
        return;
    }
    ++cursor_;
}

string Editor::GetText() const {
    return {text_.begin(), text_.end()};
}

string Editor::GetBuffer() const {
    return {buffer_.begin(), buffer_.end()};
}

int main() {
    /*
    Editor editor;
    const string text = "hello, world"s;
    for (char c : text) {
        editor.Insert(c);
    }
    // Текущее состояние редактора: `hello, world|`
    for (size_t i = 0; i < text.size(); ++i) {
        editor.Left();
    }
    
    // Текущее состояние редактора: `|hello, world`
    editor.Cut(7);
    // Текущее состояние редактора: `|world`
    // в буфере обмена находится текст `hello, `
    for (size_t i = 0; i < 5; ++i) {
        editor.Right();
    }
    // Текущее состояние редактора: `world|`
    editor.Insert(',');
    editor.Insert(' ');
    // Текущее состояние редактора: `world, |`
    editor.Paste();
    // Текущее состояние редактора: `world, hello, |`
    editor.Left();
    editor.Left();
    //Текущее состояние редактора: `world, hello|, `
    editor.Cut(3);  // Будут вырезаны 2 символа
    // Текущее состояние редактора: `world, hello|`
    cout << editor.GetText();
    return 0;
    */
    Editor editor;
    const string text = "0123456789";
    for (char c : text) {
        editor.Insert(c);
    }
    for (int i = 0; i<2; ++i) {
        editor.Left();
    }
    editor.Cut(2);
    cout << "(" << editor.GetText() << ")" << endl;
    cout << "(" << editor.GetBuffer() << ")" << endl;
    for (int i = 0; i<2; ++i) {
        editor.Left();
    }
    editor.Paste();
    cout << "(" << editor.GetText() << ")" << endl;
    cout << "(" << editor.GetBuffer() << ")" << endl;
    editor.Insert('r');
    cout << "(" << editor.GetText() << ")" << endl;
    cout << "(" << editor.GetBuffer() << ")" << endl;
    editor.Insert('k');
    cout << "(" << editor.GetText() << ")" << endl;
    cout << "(" << editor.GetBuffer() << ")" << endl;
    editor.Cut(0);
    cout << "(" << editor.GetText() << ")" << endl;
    cout << "(" << editor.GetBuffer() << ")" << endl;
    }
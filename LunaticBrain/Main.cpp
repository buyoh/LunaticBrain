
#include <iostream>
#include <fstream>
#include <cstdio>

#include <vector>
#include <string>
#include <stack>

using namespace std;


const size_t DEFAULT_MEMSIZE = 202000;




class Interpreter {
public:
    enum Status {
        success,
        halt,
        error
    };

    virtual string& source() = 0;
    virtual int operator[](int idx) = 0; // memory

                                         // check code and compile
    virtual void initialize() = 0;

    // reset interpreter
    virtual void reset() = 0;

    // execute only 1 command
    virtual Status step() = 0;
};



class Interpreter_unoptimized : public Interpreter {
private:
    int mem_ptr;
    int code_ptr;
    vector<int> mem;
    string code;

    stack<int> stk;

    inline bool issafe_memidx(int i) { return 0 <= i && i < mem.size(); }

public:

    Interpreter_unoptimized() :mem(DEFAULT_MEMSIZE) {

    }

    inline string& source() override {
        return code;
    }

    inline int operator[](int idx) override {
        return issafe_memidx(idx) ? mem[idx] : 0;
    }

    inline void initialize() override {
    }

    void reset() override {
        mem_ptr = 0;
        code_ptr = 0;
        fill(mem.begin(), mem.end(), 0);
    }


    Status step() override {
        if (code.size() <= code_ptr)
            return Status::halt;

        switch (code[code_ptr]) {
            // brainfuck
        case '>':
            ++mem_ptr;
            break;
        case '<':
            --mem_ptr;
            break;
        case '+':
            if (!issafe_memidx(mem_ptr)) return Status::error;
            ++mem[mem_ptr];
            break;
        case '-':
            if (!issafe_memidx(mem_ptr)) return Status::error;
            --mem[mem_ptr];
            break;
        case ',':
            if (!issafe_memidx(mem_ptr)) return Status::error;
            mem[mem_ptr] = getchar();
            break;
        case '.':
            if (!issafe_memidx(mem_ptr)) return Status::error;
            putchar(mem[mem_ptr] & 255);
            break;
        case '[':
            if (mem[mem_ptr] == 0)
                while (code_ptr < code.size() - 1 && code[++code_ptr] != ']');
            break;
        case ']':
            if (mem[mem_ptr] != 0)
                while (0 < code_ptr && code[--code_ptr] != '[');
            break;

            // lunatic brain basic
        case '#':
            if (!issafe_memidx(mem_ptr)) return Status::error;
            mem[mem_ptr] <<= 1;
            break;
        case '=':
            if (!issafe_memidx(mem_ptr)) return Status::error;
            mem[mem_ptr] >>= 1;
            break;
        case '^':
            if (!issafe_memidx(mem_ptr)) return Status::error;
            stk.push(mem[mem_ptr]);
            break;
        case 'v':
            if (!issafe_memidx(mem_ptr)) return Status::error;
            if (stk.empty()) return Status::error;
            mem[mem_ptr] = stk.top();
            stk.pop();
            break;
        case '$':
            stk.push(mem_ptr);
            break;
        case '*':
            if (stk.empty()) return Status::error;
            mem_ptr = stk.top();
            stk.pop();
            break;
        case 'i':
            stk.push(code_ptr);
            break;
        case '!':
            if (stk.empty()) return Status::error;
            code_ptr = stk.top();
            stk.pop();
            break;


            // lunatic brain extended
        case 'p':
            if (!issafe_memidx(mem_ptr)) return Status::error;
            printf("%d ", mem[mem_ptr]);
            break;
        case 's':
            if (!issafe_memidx(mem_ptr)) return Status::error;
            scanf_s("%d", &mem[mem_ptr]);
            break;
        }



        ++code_ptr;
        return Status::success;
    }

};


[[noreturn]] void abort_msg(string str) {
    clog << str << endl;
    abort();
}


void print_help() {
    cout << "Lunatic Brain \n";
    cout << "Version 0.0.0 (" << __DATE__ << ")\n";
    cout << "- - - - - - - - -\n";
    cout << endl;
}


void execute(char* filename) {

    ifstream fin(filename);

    if (fin.fail())
        abort_msg("failure: fileopen");

    auto vm = Interpreter_unoptimized();

    {
        char buff[4096];
        while (!fin.eof()) {
            fin.read(buff, 4096);

            vm.source().append(buff, buff + fin.gcount());
        }
    }

    vm.reset();
    while (vm.step() == Interpreter::Status::success);

    if (vm.step() == Interpreter::Status::error) {
        abort_msg("error: runtime error");
    }
}


int main(int argc, char** argv) {

    char* filename = nullptr;
    int mode = 0;


    for (int idx = 1; idx < argc; ++idx) {

        if (argv[idx][0] != '-') {
            filename = argv[idx];
            continue;
        }

        if (strcmp("--help", argv[idx]) == 0 ||
            strcmp("-h", argv[idx]) == 0 ||
            strcmp("--version", argv[idx]) == 0 ||
            strcmp("-v", argv[idx]) == 0) {
            filename = nullptr;
            break;
        }
    }

    if (filename == nullptr) {
        print_help();
        return 0;
    }
    execute(filename);

    return 0;
}
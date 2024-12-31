/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *              REFACTORING OF corretor.c
 *  
 *                @author santosantonio1
 *                version: 31/12/2024
 *  
 *          idk i just wanted to code something...
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <iostream>
#include <filesystem>
#include <memory>
#include <vector>
#include <fstream>

#define GCC_RET_VALUE   0
#define COMPILER        "g++ "
#define CFLAGS           " -std=c++23 "

#define PROG_DIR        "Aluno"
#define INPUT_DIR       "input"
#define OUTPUT_DIR      "output"
#define ANS_DIR         "ans"

class Corretor {
    public:
        Corretor(const std::string& p, const std::string& i, 
                 const std::string& o, const std::string& a) {
            prog_p.append(p);
            in_p.append(i);
            out_p.append(o);
            ans_p.append(a);

            in = std::filesystem::directory_iterator{in_p};
            std::filesystem::create_directory(out_p);
        }
        auto run() -> void {
            for(const auto& file : std::filesystem::directory_iterator{prog_p}) {
                prog = file.path().string();

                if(!prog.ends_with(".cc") && !prog.ends_with(".cpp")) 
                        continue;

                const auto prog_name = test_name(file.path().filename().string());
                
                std::cout << "\n-------------------------------------\n";
                std::cout << '\t' << prog_name << '\n';
                std::cout << "-------------------------------------\n";

                std::cout << "\n$ Compiling...\n";

                scores.push_back(std::pair<size_t, size_t>{});
                const size_t id = scores.size()-1;

                if(compile(prog_name) == GCC_RET_VALUE) {
                    std::cout << "Compiled!\n";
                    std::cout << "\n$ Running testcases\n\n";

                    // obs: if the first condition fails, the loop exits and doesn't 
                    // even read the others
                    while(std::filesystem::end(in) != in && 
                          in->path().filename().string().contains(prog_name)) {
                        testcase = in->path().filename().string();
                        scores[id].second++;
                        execute(prog_name);

                        const int x = check(testcase);
                        
                        std::cout << '[' << testcase;
                        if(x == 1) {
                            scores[id].first++;
                            std::cout << "] - PASSED\n";
                        }
                        else {
                            std::cout << "] - FAILED\n";
                        }

                        in++;
                    }
                    
                    std::string rm = "del ";
                    rm.append(prog_name);
                    rm.append(".exe");
                    system(rm.c_str());
                }
                else {
                    // the show MUST go on!!!
                   while(std::filesystem::end(in) != in && 
                         in->path().filename().string().contains(prog_name)) {
                        scores[id].second++;
                        in++; 
                    }
                    std::cout << "\n(!) Compilation failed for " << prog_name << " :(\n";
                }

                std::cout << "\n\t(#) SCORE: " << scores[id].first << '/' << scores[id].second << '\n';
            } 
        }
        auto performance() const -> void {
            std::cout << "\n$ Calculating grade...\n";
            std::pair<size_t, size_t> grade{};
            for(const auto& p : scores) {
                grade.first += p.first;
                grade.second += p.second;
            }

            std::cout << "\n\tOVERALL:";
            std::cout << '\t' << grade.first << '/' << grade.second << '\n';
        }
    private:
        std::filesystem::path prog_p;
        std::filesystem::path in_p;
        std::filesystem::path out_p;
        std::filesystem::path ans_p;

        std::filesystem::directory_iterator in;

        std::string prog;
        std::string testcase;

        std::vector<std::pair<size_t, size_t>> scores;

        auto test_name(const std::string& filename) -> std::string {
            auto s = filename;
            size_t idx = 0;
            if(s.ends_with(".cc")) {
                idx = s.find(".cc");
            }
            else if(s.ends_with(".cpp")) {
                idx = s.find(".cpp");
            }
            s.erase(s.begin()+idx, s.end());

            return s;
        }

        auto compile(const std::string& prog_name) -> int {
            std::string cmd = COMPILER;
            cmd.append(prog);
            cmd.append(" -o ");
            cmd.append(prog_name);
            cmd.append(CFLAGS);

            // std::cout << cmd << '\n';

            return system(cmd.c_str());
        }

        auto execute(const std::string& prog_name) -> int {
            std::string cmd = prog_name;

            cmd.append(" < ");
            cmd.append(in->path().string());
            cmd.append(" > ");

            out_p.append(testcase);
            cmd.append(out_p.string());
            // out_p.remove_filename();

            // std::cout << cmd << '\n';

            return system(cmd.c_str());
        }

        auto check(const std::string& testcase) -> int {
            int score = 1;

            ans_p.append(testcase);

            std::ifstream out_file(out_p.string()), ans_file(ans_p.string());
            std::string out, tar;
            
            while(!ans_file.eof()) {
                std::getline(ans_file, tar);
                std::getline(out_file, out);
                if(out != tar) {
                    // hopefully ignores the last whitespace in each line
                    if(out.length() == tar.length()+1 && out.back() == ' ') {
                        continue;
                    }
                    score = 0;
                    break;
                }
            }

            // hopefully ignores '\n' at eof
            if(!out_file.eof()) {
                getline(out_file, out);
                if(out != "") {
                    score = 0;
                }
            }

            out_p.remove_filename();
            ans_p.remove_filename();
            out_file.close();
            ans_file.close();
            
            return score;
        }
};

int main()
{
    Corretor c(PROG_DIR, INPUT_DIR, OUTPUT_DIR, ANS_DIR);
    c.run();
    c.performance();
    
    return 0;
}
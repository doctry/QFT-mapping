#pragma once

#include <math.h>
#include <unistd.h>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <ios>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

// This part is adapted from https://github.com/aminnj/cpptqdm
class tqdm {
   private:
    // time, iteration counters and deques for rate calculations
    std::chrono::time_point<std::chrono::system_clock> t_first_ =
        std::chrono::system_clock::now();
    std::chrono::time_point<std::chrono::system_clock> t_old_ =
        std::chrono::system_clock::now();
    int n_old_ = 0;
    std::vector<double> deq_t_;
    std::vector<int> deq_n_;
    int nupdates_ = 0;
    int total_ = 0;
    int period_ = 1;
    size_t smoothing_ = 50;
    bool use_ema_ = true;
    float alpha_ema_ = 0.1;

    std::vector<const char*> bars_ = {" ", "▏", "▎", "▍", "▌",
                                      "▋", "▊", "▉", "█"};

    bool in_screen_ = (system("test $STY") == 0);
    bool in_tmux_ = (system("test $TMUX") == 0);
    bool is_tty_ = isatty(1);
    bool use_colors_ = true;
    bool color_transition_ = true;
    int width_ = 40;

    std::string right_pad_ = "▏";
    std::string label_ = "";

    void hsv_to_rgb(float h, float s, float v, int& r, int& g, int& b) const;

   public:
    tqdm() : deq_t_({}), deq_n_({}) {
        if (in_screen_) {
            set_theme_basic();
            color_transition_ = false;
        } else if (in_tmux_) {
            color_transition_ = false;
        }
    }

    void reset() {
        t_first_ = std::chrono::system_clock::now();
        t_old_ = std::chrono::system_clock::now();
        n_old_ = 0;
        deq_t_.clear();
        deq_n_.clear();
        period_ = 1;
        nupdates_ = 0;
        total_ = 0;
        label_ = "";
    }

    void set_theme_line() {
        bars_ = {"─", "─", "─", "╾", "╾", "╾", "╾", "━", "═"};
    }
    void set_theme_circle() {
        bars_ = {" ", "◓", "◑", "◒", "◐", "◓", "◑", "◒", "#"};
    }
    void set_theme_braille() {
        bars_ = {" ", "⡀", "⡄", "⡆", "⡇", "⡏", "⡟", "⡿", "⣿"};
    }
    void set_theme_braille_spin() {
        bars_ = {" ", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠇", "⠿"};
    }
    void set_theme_vertical() {
        bars_ = {"▁", "▂", "▃", "▄", "▅", "▆", "▇", "█", "█"};
    }
    void set_theme_basic() {
        bars_ = {" ", " ", " ", " ", " ", " ", " ", " ", "#"};
        right_pad_ = "|";
    }
    void set_label(std::string label) { label_ = label; }
    void disable_colors() {
        color_transition_ = false;
        use_colors_ = false;
    }

    void finish() {
        progress(total_, total_);
        printf("\n");
        fflush(stdout);
    }
    void progress(int curr, int tot);
};

class Tqdm {
   public:
    Tqdm(size_t total) : counter_(0), total_(total), tqdm_(tqdm()) {}
    ~Tqdm() { tqdm_.finish(); }

    size_t idx() const { return counter_; }
    bool done() const { return counter_ == total_; }
    void add() { tqdm_.progress(counter_++, total_); }

   private:
    size_t counter_;
    size_t total_;
    tqdm tqdm_;
};

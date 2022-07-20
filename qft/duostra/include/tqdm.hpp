#pragma once

#ifndef TQDM_H
#define TQDM_H
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

class tqdm {
   private:
    // time, iteration counters and deques for rate calculations
    std::chrono::time_point<std::chrono::system_clock> t_first =
        std::chrono::system_clock::now();
    std::chrono::time_point<std::chrono::system_clock> t_old =
        std::chrono::system_clock::now();
    int n_old = 0;
    std::vector<double> deq_t;
    std::vector<int> deq_n;
    int nupdates = 0;
    int total_ = 0;
    int period = 1;
    unsigned int smoothing = 50;
    bool use_ema = true;
    float alpha_ema = 0.1;

    std::vector<const char*> bars = {" ", "▏", "▎", "▍", "▌",
                                     "▋", "▊", "▉", "█"};

    bool in_screen = (system("test $STY") == 0);
    bool in_tmux = (system("test $TMUX") == 0);
    bool is_tty = isatty(1);
    bool use_colors = true;
    bool color_transition = true;
    int width = 40;

    std::string right_pad = "▏";
    std::string label = "";

    void hsv_to_rgb(float h, float s, float v, int& r, int& g, int& b) const;

   public:
    tqdm() {
        if (in_screen) {
            set_theme_basic();
            color_transition = false;
        } else if (in_tmux) {
            color_transition = false;
        }
    }

    void reset() {
        t_first = std::chrono::system_clock::now();
        t_old = std::chrono::system_clock::now();
        n_old = 0;
        deq_t.clear();
        deq_n.clear();
        period = 1;
        nupdates = 0;
        total_ = 0;
        label = "";
    }

    void set_theme_line() {
        bars = {"─", "─", "─", "╾", "╾", "╾", "╾", "━", "═"};
    }
    void set_theme_circle() {
        bars = {" ", "◓", "◑", "◒", "◐", "◓", "◑", "◒", "#"};
    }
    void set_theme_braille() {
        bars = {" ", "⡀", "⡄", "⡆", "⡇", "⡏", "⡟", "⡿", "⣿"};
    }
    void set_theme_braille_spin() {
        bars = {" ", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠇", "⠿"};
    }
    void set_theme_vertical() {
        bars = {"▁", "▂", "▃", "▄", "▅", "▆", "▇", "█", "█"};
    }
    void set_theme_basic() {
        bars = {" ", " ", " ", " ", " ", " ", " ", " ", "#"};
        right_pad = "|";
    }
    void set_label(std::string label_) { label = label_; }
    void disable_colors() {
        color_transition = false;
        use_colors = false;
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
    Tqdm(long total) : counter_(0), total_(total), tqdm_(tqdm()) {}
    ~Tqdm() { tqdm_.finish(); }

    void add() { tqdm_.progress(counter_++, total_); }

   private:
    long counter_;
    long total_;
    tqdm tqdm_;
};
#endif

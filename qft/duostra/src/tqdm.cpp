#include "tqdm.hpp"

void tqdm::hsv_to_rgb(float h, float s, float v, int& r, int& g, int& b) const {
    if (s < 1e-6) {
        v *= 255.;
        r = v;
        g = v;
        b = v;
    }
    int i = (int)(h * 6.0);
    float f = (h * 6.) - i;
    int p = (int)(255.0 * (v * (1. - s)));
    int q = (int)(255.0 * (v * (1. - s * f)));
    int t = (int)(255.0 * (v * (1. - s * (1. - f))));
    v *= 255;
    i %= 6;
    int vi = (int)v;
    if (i == 0) {
        r = vi;
        g = t;
        b = p;
    } else if (i == 1) {
        r = q;
        g = vi;
        b = p;
    } else if (i == 2) {
        r = p;
        g = vi;
        b = t;
    } else if (i == 3) {
        r = p;
        g = q;
        b = vi;
    } else if (i == 4) {
        r = t;
        g = p;
        b = vi;
    } else if (i == 5) {
        r = vi;
        g = p;
        b = q;
    }
}

void tqdm::progress(int curr, int tot) {
    if (is_tty_ && (curr % period_ == 0)) {
        total_ = tot;
        nupdates_++;
        auto now = std::chrono::system_clock::now();
        double dt = ((std::chrono::duration<double>)(now - t_old_)).count();
        double dt_tot =
            ((std::chrono::duration<double>)(now - t_first_)).count();
        int dn = curr - n_old_;
        n_old_ = curr;
        t_old_ = now;
        if (deq_n_.size() >= smoothing_)
            deq_n_.erase(deq_n_.begin());
        if (deq_t_.size() >= smoothing_)
            deq_t_.erase(deq_t_.begin());
        deq_t_.push_back(dt);
        deq_n_.push_back(dn);

        double avgrate = 0.;
        if (use_ema_) {
            avgrate = deq_n_[0] / deq_t_[0];
            for (size_t i = 1; i < deq_t_.size(); i++) {
                double r = 1.0 * deq_n_[i] / deq_t_[i];
                avgrate = alpha_ema_ * r + (1.0 - alpha_ema_) * avgrate;
            }
        } else {
            double dtsum = std::accumulate(deq_t_.begin(), deq_t_.end(), 0.);
            int dnsum = std::accumulate(deq_n_.begin(), deq_n_.end(), 0.);
            avgrate = dnsum / dtsum;
        }

        // learn an appropriate period length to avoid spamming stdout
        // and slowing down the loop, shoot for ~25Hz and smooth over 3
        // seconds
        if (nupdates_ > 10) {
            period_ =
                (int)(std::min(std::max((1.0 / 25) * curr / dt_tot, 1.0), 5e5));
            smoothing_ = 25 * 3;
        }
        double peta = (tot - curr) / avgrate;
        double pct = (double)curr / (tot * 0.01);
        if ((tot - curr) <= period_) {
            pct = 100.0;
            avgrate = tot / dt_tot;
            curr = tot;
            peta = 0;
        }

        double fills = ((double)curr / tot * width_);
        int ifills = (int)fills;

        printf("\015 ");
        if (use_colors_) {
            if (color_transition_) {
                // red (hue=0) to green (hue=1/3)
                int r = 255, g = 255, b = 255;
                hsv_to_rgb(0.0 + 0.01 * pct / 3, 0.65, 1.0, r, g, b);
                printf("\033[38;2;%d;%d;%dm ", r, g, b);
            } else {
                printf("\033[32m ");
            }
        }
        for (int i = 0; i < ifills; i++)
            std::cout << bars_[8];
        if (!in_screen_ and (curr != tot))
            printf("%s", bars_[(int)(8.0 * (fills - ifills))]);
        for (int i = 0; i < width_ - ifills - 1; i++)
            std::cout << bars_[0];
        printf("%s ", right_pad_.c_str());
        if (use_colors_)
            printf("\033[1m\033[31m");
        printf("%4.1f%% ", pct);
        if (use_colors_)
            printf("\033[34m");

        std::string unit = "Hz";
        double div = 1.;
        if (avgrate > 1e6) {
            unit = "MHz";
            div = 1.0e6;
        } else if (avgrate > 1e3) {
            unit = "kHz";
            div = 1.0e3;
        }
        printf("[%4d/%4d | %3.1f %s | %.0fs<%.0fs] ", curr, tot, avgrate / div,
               unit.c_str(), dt_tot, peta);
        printf("%s ", label_.c_str());
        if (use_colors_)
            printf("\033[0m\033[32m\033[0m\015 ");

        if ((tot - curr) > period_)
            fflush(stdout);
    }
}

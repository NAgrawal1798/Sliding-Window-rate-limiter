#include <iostream>
#include <unordered_map>
#include <ctime>
#include <chrono>
#include <thread>

class SlidingWindowRateLimiter {
public:
    SlidingWindowRateLimiter(int limit, int window_size) : limit(limit), window_size(window_size) {}

    bool isAllowed() {
        time_t current_time = time(nullptr);
        time_t window_start_time = current_time - window_size + 1;
        std::cout<<window_start_time<<std::endl;
        // Remove old timestamps
        for (auto it = requests.begin(); it != requests.end();) {
            if (it->first < window_start_time) {
                it = requests.erase(it);
            } else {
                ++it;
            }
        }

        // Check if the number of requests is within the limit
        int request_count = 0;
        for (const auto& entry : requests) {
            request_count += entry.second;
        }

        if (request_count < limit) {
            // Increment the request count for the current timestamp
            requests[current_time]++;
            return true;
        } else {
            return false;
        }
    }

private:
    int limit;
    int window_size;
    std::unordered_map<time_t, int> requests;
};

int main() {
    SlidingWindowRateLimiter rateLimiter(2, 50);  // Allow 5 requests per minute

    for (int i = 0; i < 10; ++i) {
        if (rateLimiter.isAllowed()) {
            std::cout << "Request " << i + 1 << ": Allowed\n";
        } else {
            std::cout << "Request " << i + 1 << ": Denied\n";
        }

        // Simulate a delay between requests
        std::chrono::seconds sleep_duration(10);
        std::this_thread::sleep_for(sleep_duration);;
    }

    return 0;
}

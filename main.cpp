#include <Eigen/Dense>
#include "webview/webview.h"
#include "ScreenUtils.hpp"
#include "StringUtils.hpp"
#include "Log.hpp"
#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>

namespace gk = groklab;

using Eigen::MatrixXd;
using Eigen::VectorXd;

void testWebview() {
    const gk::ScreenSize screenSize = gk::getScreenSize();

    std::cout << "Screen Width: " << screenSize.width << ", Screen Height: " << screenSize.height << std::endl;
    try {
        long count = 0;
        const int heightDiv = 1;
        const int widthDiv = 1;

        // Read the content of test.html
        std::ifstream file("./web/index.html");
        if (!file) {
            std::cerr << "Could not open the file!" << std::endl;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string html = buffer.str();

        webview::webview w(true, nullptr);
        w.set_title("Bind Example");
        w.set_size(screenSize.width/widthDiv, screenSize.height/heightDiv, WEBVIEW_HINT_NONE);

        // A binding that counts up or down and immediately returns the new value.
        w.bind("count", [&](const std::string &req) -> std::string {
          // Imagine that req is properly parsed or use your own JSON parser.
          auto direction = std::stol(req.substr(1, req.size() - 1));
          return std::to_string(count += direction);
        });

        // A binding that creates a new thread and returns the result at a later time.
        w.bind(
            "compute",
            [&](const std::string &id, const std::string &req, void * /*arg*/) {
              // Create a thread and forget about it for the sake of simplicity.
              std::thread([&, id, req] {
                // Simulate load.
                std::this_thread::sleep_for(std::chrono::seconds(1));
                // Imagine that req is properly parsed or use your own JSON parser.
                const auto *result = "42";
                w.resolve(id, 0, result);
              }).detach();
            },
            nullptr);

        w.set_html(html);
        w.run();
    } catch (const webview::exception &e) {
        gk::critical("Failed to initialize FluidUI with error {}", e.what());
    }
}

void testEigen() {
    MatrixXd m = MatrixXd::Random(3,3);
    m = (m + MatrixXd::Constant(3,3,1.2)) * 50;
    gk::info("m = \n{}", gk::StringUtils::toString(m));
    VectorXd v(3);
    v << 1, 2, 3;
    gk::info("m * v =\n{}", gk::StringUtils::toString(m * v));
}

int main() {
    // testWebview();
    testEigen();
    return 0;
}
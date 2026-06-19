#include "Application.h"

int main() {

    const auto application = std::make_unique<Application>();
    application->run();

    return 0;
}
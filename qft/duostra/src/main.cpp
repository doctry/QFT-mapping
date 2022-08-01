#include "flow.hpp"
#include "sa_place.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./qft_mapping <config.json>";
        return 1;
    }

    // config file
    std::ifstream ifs(argv[1]);
    json conf = json::parse(ifs);

    json mapper_conf = json_get<json>(conf, "mapper");

    // flow
    if (json_get<std::string>(mapper_conf, "placer") == "sa") {
        sa_place(conf);
    } else {
        std::vector<size_t> dummy;
        flow(conf, dummy, true);
    }
    return 0;
}

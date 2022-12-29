#include <iostream>

#include <boost/program_options.hpp>

class Arguments {
  friend std::ostream &operator<<(std::ostream &os,
                                  const Arguments &arguments) {
    os << "Arguments:" << std::endl
       << "  data_pointed_path = \"" << arguments.data_pointed_path << "\""
       << std::endl
       << "  data_visited_path = \"" << arguments.data_visited_path << "\""
       << std::endl;
    return os;
  }

 public:
  const std::string data_pointed_path = "";
  const std::string data_visited_path = "";

  explicit Arguments(const std::string &data_pointed_path_,
                     const std::string &data_visited_path_)
      : data_pointed_path{data_pointed_path_},
        data_visited_path{data_visited_path_},
        has_error_{false} {}

  bool HasError() { return has_error_; }

  static Arguments WithError(const std::string &message) {
    return Arguments(message);
  }

 private:
  bool has_error_;
  std::string error_message_;

  explicit Arguments(const std::string &error_message)
    : has_error_{true}, error_message_{error_message} {}
};

[[nodiscard]] static inline Arguments ParseArguments(int argc, char *argv[]) {
  namespace po = boost::program_options;

  po::options_description desc("calor options");
  desc.add_options()("help", "produce help message")
    ("data_pointed",
     po::value<std::string>()->required(),
     "Path to data pointed csv")
    ("data_visited",
     po::value<std::string>()->required(),
     "Path to data visited csv");

  po::variables_map vm;

  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
  } catch (po::invalid_command_line_syntax &error) {
    std::cout << error.what() << std::endl << std::endl << desc << std::endl;
    return Arguments::WithError("bad argument");
  }

  try {
    po::notify(vm);
  } catch (po::required_option &error) {
    std::cout << error.what() << std::endl << std::endl << desc << std::endl;
    return Arguments::WithError("required argument");
  }

  return Arguments{
    vm["data_pointed"].as<std::string>(),
    vm["data_visited"].as<std::string>()
  };
}

int main(int argc, char *argv[]) {
  Arguments arguments = ParseArguments(argc, argv);
  if (arguments.HasError()) {
    return EXIT_FAILURE;
  }

  std::cout << arguments << std::endl;

  return EXIT_SUCCESS;
}

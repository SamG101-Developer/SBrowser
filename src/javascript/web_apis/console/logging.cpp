#include "logging.hpp"

#include <iostream>

#include <ext/iterables/range.hpp>

#include <console/abstract.hpp>


template <typename ...data_t>
void
console::logging::assert_(
        bool condition,
        data_t&& ...data) {

    if (condition) return;


    ext::vector<ext::string> data_list = {data...};
    data_list.front() = "Assertion Failed";
    data_list.front() += not data_list.empty() ? (ext::string)": " + data_list.front() : " ";

    abstract::logger("assert", data_list);
}


template <typename ...data_t>
void
console::logging::debug(data_t&& ...data) {
    abstract::logger("debug", ext::vector<ext::string>{data...});
}


template <typename ...data_t>
void
console::logging::error(data_t&& ...data) {
    abstract::logger("error", ext::vector<ext::string>{data...});
}


template <typename ...data_t>
void
console::logging::info(data_t&& ...data) {
    abstract::logger("info", ext::vector<ext::string>{data...});
}


template <typename ...data_t>
void
console::logging::log(data_t&& ...data) {
    abstract::logger("log", ext::vector<ext::string>{data...});
}


template <typename ...data_t>
void
console::logging::warn(data_t&& ...data) {
    abstract::logger("warn", ext::vector<ext::string>{data...});
}


template <typename ...data_t>
void
console::logging::trace(data_t&& ...data) {
    auto trace = v8::StackTrace::CurrentStackTrace(v8::Isolate::GetCurrent(), 20);
    auto formatted_data = abstract::formatter(ext::vector<ext::string>{data...});
    // TODO -> set the trace label to formatted_data

    abstract::printer("trace", {trace});
}


template <typename ...data_t>
void
console::logging::dir_xml(data_t&& ...data) {
    ext::vector<ext::string> data_list{data...};
    ext::vector<ext::string> final_list {};
    // TODO ?
    final_list = data_list;
    abstract::printer("dirxml", data_list);
}


void
console::logging::table(
        const ext::any& tabular_data,
        const ext::vector<ext::string>& properties_list) {

    // array of strings ie ["apples", "oranges", "bananas"]
    if (ext::vector<ext::string> array = tabular_data) {
        auto indexes = ext::range<size_t>{0, array.length()};
        auto columns = ext::vector<ext::vector<ext::string>>{indexes, tabular_data};
        // helpers::format_table(columns); TODO
    }

    // object whose properties are strings

    // array of arrays
    else if (ext::vector<ext::vector<ext::string>> compound_array = tabular_data) {
        auto sizes = compound_array.transform<size_t>([](const ext::vector<ext::string>& array) -> size_t {return array.length();});
        auto max_column_count = sizes.max_element();

        compound_array.for_each([max_column_count](ext::vector<ext::string>& array) -> void {
            for (auto i = array.length(); i <= max_column_count; ++i) array.append("");
        });

        auto columns = compound_array;
        //helpers::format_table(columns); TODO
    }

    // array of objects

    // object whose properties are objects

    // TODO : properties_list restricts the columns to print (only for object mode ie the name of the attr to print)
}


void
console::logging::dir(
        const ext::any& item,
        const ext::any& options) {

    // TODO : generic JavaScript object formatting (JSON repr of JS-converted object?)
    auto object = item;

    abstract::printer("dir", {object}, options);
}


void
console::logging::clear() {
    while (not group_stack.empty()) group_stack.pop();
    std::cout << std::flush;
    system("cls");
}

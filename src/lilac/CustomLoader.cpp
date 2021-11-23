#include <CustomLoader.hpp>
#include <Loader.hpp>

USE_LILAC_NAMESPACE();

void CustomLoader::addSearchPath(std::string const& path) {
    Loader::get()->m_searchPaths.push_back(path);
}

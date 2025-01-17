#pragma once

#include <vcpkg/base/fwd/expected.h>

#include <vcpkg/fwd/triplet.h>
#include <vcpkg/fwd/vcpkgcmdarguments.h>
#include <vcpkg/fwd/vcpkgpaths.h>

#include <string>
#include <unordered_set>
#include <vector>

namespace vcpkg
{
 
    extern const CommandMetadata CommandDependOnMetadata;

    void command_depend_on_and_exit(const VcpkgCmdArguments& args,
                                      const VcpkgPaths& paths,
                                      Triplet default_triplet,
                                      Triplet host_triplet);
}

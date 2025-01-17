#include <vcpkg/base/contractual-constants.h>
#include <vcpkg/base/expected.h>
#include <vcpkg/base/strings.h>
#include <vcpkg/base/util.h>
#include <vcpkg/base/xmlserializer.h>

#include <vcpkg/cmakevars.h>
#include <vcpkg/commands.depend-on.h>
#include <vcpkg/dependencies.h>
#include <vcpkg/input.h>
#include <vcpkg/packagespec.h>
#include <vcpkg/portfileprovider.h>
#include <vcpkg/registries.h>
#include <vcpkg/triplet.h>
#include <vcpkg/vcpkgcmdarguments.h>

#include <limits.h>

using namespace vcpkg;

namespace vcpkg
{
    constexpr CommandMetadata CommandDependOnMetadata{
        "depend-on",
        msgHelpDependOnCommand,
        {msgCmdDependOnExample1, "vcpkg depend-on zlib"},
        "https://learn.microsoft.com/vcpkg/commands/depend-info",
        AutocompletePriority::Public,
        1,
        1,
        {},
        nullptr,
    };

    void command_depend_on_and_exit(const VcpkgCmdArguments& args,
                                      const VcpkgPaths& paths,
                                      Triplet default_triplet,
                                      Triplet host_triplet)
    {
        msg::default_output_stream = OutputStream::StdErr;
        const ParsedArguments options = args.parse_arguments(CommandDependOnMetadata);
        auto& fs = paths.get_filesystem();
        auto registry_set = paths.make_registry_set();
        PathsPortFileProvider provider(*registry_set, make_overlay_provider(fs, paths.overlay_ports));
        auto source_paragraphs =
            Util::fmap(provider.load_all_control_files(),
                       [](auto&& port) -> const SourceControlFile* { return port->source_control_file.get(); });
        const auto filter = options.command_arguments[0];
        for (const auto& source_control_file : source_paragraphs)
        {
            for (const auto& dependency : source_control_file->core_paragraph->dependencies)
            {
                if (dependency.name == filter)
                {
                    msg::write_unlocalized_text_to_stdout(Color::none, source_control_file->core_paragraph->name);
                    msg::write_unlocalized_text_to_stdout(Color::none, "\n");
                }
            }
            auto&& sp = *source_control_file->core_paragraph;
            if (sp.name == filter)
            {
                for (auto&& feature_paragraph : source_control_file->feature_paragraphs)
                {
                    msg::write_unlocalized_text_to_stdout(Color::none, feature_paragraph->name);
                    msg::write_unlocalized_text_to_stdout(Color::none, "\n");
                }
            }
        }
        Checks::exit_success(VCPKG_LINE_INFO);
    }
} // namespace vcpkg

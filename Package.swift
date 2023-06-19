// swift-tools-version: 5.4

import Foundation
import PackageDescription

let package = Package(
    name: "flite",
    products: [
        .library(
            name: "flite",
            targets: ["flite"])
    ],
    dependencies: [
    ],
    targets: [
        .target(
            name: "flite",
            dependencies: [
            ],
            path:"",
            exclude:[
                // Next files designed for specific OS and can't be built on Apple platforms
                "src/audio/au_pulseaudio.c",
                "src/audio/au_win.c",
                "src/audio/au_alsa.c",
                "src/audio/au_oss.c",
                "src/audio/au_wince.c",
                "src/audio/au_command.c",
                "src/audio/au_palmos.c",
                "src/audio/au_sun.c",

                "src/utils/cst_file_wince.c",
                "src/utils/cst_file_palmos.c",
                "src/utils/cst_mmap_none.c",
                "src/utils/cst_file_wince.c",
                "src/utils/cst_mmap_win32.c",

                "lang/cmu_us_rms/cmu_us_rms_voice.cpp",
                "lang/cmulex/cmu_lex_num_bytes.c",
                "lang/cmulex/cmu_lex_phones_huff_table.c",
                "lang/cmulex/cmu_lex_entries_huff_table.c",
                "lang/cmulex/cmu_lex_data_raw.c",
                // Makefiles shoundn't be "build"
                "src/Makefile",
                "src/cg/Makefile",
                "src/hrg/Makefile",
                "src/lexicon/Makefile",
                "src/regex/Makefile",
                "src/speech/Makefile",
                "src/stats/Makefile",
                "src/synth/Makefile",
                "src/utils/Makefile",
                "src/wavesynth/Makefile",
                "lang/cmu_indic_lang/Makefile",
                "lang/cmu_indic_lex/Makefile",
                "lang/cmu_time_awb/Makefile",
                "lang/cmu_us_awb/Makefile",
                "lang/cmu_us_kal/Makefile",
                "lang/cmu_us_kal16/Makefile",
                "lang/cmu_us_rms/Makefile",
                "lang/cmu_us_slt/Makefile",
                "lang/cmulex/Makefile",
                "src/audio/Makefile",
                "lang/usenglish/Makefile",
                "lang/cmulex/make_cmulex",
                "lang/usenglish/make_us_regexes",
                "lang/usenglish/us_pos.tree",
                // vcproj - Windows visual studio files
                "lang/cmulex/libflite-cmulex.vcproj",
                "lang/cmu_us_rms/cmu_us_rms.vcproj",
                "lang/usenglish/libflite-usenglish.vcproj"
            ],
            sources:[
                // Main sources
                "src",
                // Voices sourses
                "lang/cmu_us_slt/",
                "lang/cmu_us_awb/",
                "lang/cmu_indic_lang/",
                "lang/cmu_indic_lex/",
                "lang/cmu_time_awb/",
                "lang/cmu_us_awb/",
                "lang/cmu_us_kal/",
                "lang/cmu_us_kal16/",
                "lang/cmu_us_rms/",
                "lang/usenglish/",
                "lang/cmulex/"
            ],
            publicHeadersPath:"include/",
            cSettings: [
                .headerSearchPath("include/**"),
                .headerSearchPath("lang/**"),
                .headerSearchPath("lang/usenglish/"),
                .headerSearchPath("lang/cmulex/"),
            ]
        )
    ],
    cLanguageStandard: .c17,
    cxxLanguageStandard: .cxx17
)

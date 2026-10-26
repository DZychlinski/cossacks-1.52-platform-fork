# Cossacks: The Last Stand - Nation AI DLLs (AI Pack v0.995 sources)
# One DLL per nation, built only for MSVC. Output goes to ai_rebuilt/ next to
# dmcr.exe so the shipped DLLs in ai/ are never overwritten.

set(AI_NATIONS
    NATIONS/Austria NATIONS/Bavaria NATIONS/Denmark NATIONS/England
    NATIONS/France NATIONS/German NATIONS/Holland NATIONS/Piemont
    NATIONS/Poland NATIONS/Portugalia NATIONS/Russia NATIONS/Saksinia
    NATIONS/Spain NATIONS/Sveden NATIONS/Venecia
    Switzerland/Switzerland Vengria/Vengria
)

foreach(AI_SRC ${AI_NATIONS})
    get_filename_component(AI_NAME ${AI_SRC} NAME)
    string(TOUPPER ${AI_NAME} AI_NAME_UPPER)

    add_library(AI_${AI_NAME} SHARED ${AI_SRC}.cpp)

    target_compile_definitions(AI_${AI_NAME} PRIVATE
        _WINDOWS _MBCS _USRDLL ${AI_NAME_UPPER}_EXPORTS
        _CRT_SECURE_NO_WARNINGS
    )
    # VC6 scoping of for-loop variables (they leak into the enclosing scope)
    target_compile_options(AI_${AI_NAME} PRIVATE /Zc:forScope-)
    target_include_directories(AI_${AI_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

    # No link to dmcr: the AI resolves engine functions at runtime with
    # GetProcAddress(GetModuleHandle("DMCR.EXE"), ...) (Ai.h), so the exe must stay dmcr.exe.
    # Ai.h has a pragma comment(lib) for the old VC6 dmcr.lib in the parent folder: ignore it
    target_link_options(AI_${AI_NAME} PRIVATE [=[/NODEFAULTLIB:..\dmcr.lib]=])

    set_target_properties(AI_${AI_NAME} PROPERTIES
        OUTPUT_NAME ${AI_NAME}
        PREFIX ""
        # Static CRT, same as the original /MT
        MSVC_RUNTIME_LIBRARY "MultiThreaded"
        RUNTIME_OUTPUT_DIRECTORY "$<TARGET_FILE_DIR:dmcr>/ai_rebuilt"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "$<TARGET_FILE_DIR:dmcr>/ai_rebuilt"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "$<TARGET_FILE_DIR:dmcr>/ai_rebuilt"
    )
endforeach()

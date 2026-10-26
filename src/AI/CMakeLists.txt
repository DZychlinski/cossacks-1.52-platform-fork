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

# Debug chat: the AI announces in the chat which buildings it decides to build (Ai.h, AI_DEBUG_CHAT);
# also shows that the rebuilt DLLs are the ones in use.
option(COS_AI_DEBUG_CHAT "AI reports its building decisions in the in-game chat" OFF)

foreach(AI_SRC ${AI_NATIONS})
    get_filename_component(AI_NAME ${AI_SRC} NAME)
    string(TOUPPER ${AI_NAME} AI_NAME_UPPER)

    add_library(AI_${AI_NAME} SHARED ${AI_SRC}.cpp)

    target_compile_definitions(AI_${AI_NAME} PRIVATE
        _WINDOWS _MBCS _USRDLL ${AI_NAME_UPPER}_EXPORTS
        _CRT_SECURE_NO_WARNINGS
    )
    if(COS_AI_DEBUG_CHAT)
        target_compile_definitions(AI_${AI_NAME} PRIVATE AI_DEBUG_CHAT)
    endif()
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
    list(APPEND AI_TARGETS AI_${AI_NAME})
endforeach()

# Deploy the rebuilt DLLs into ai/ next to the exe, replacing the shipped ones for these 17 nations
# (Algeria, Turcia and Ukraine have no source and keep the shipped DLLs). Runs after dmcr, whose
# post-build step copies the shipped ai/ folder, so the rebuilt DLLs always win. OFF = shipped AI;
# dmcr's next relink then restores the shipped DLLs.
option(COS_USE_REBUILT_AI "Play against the AI DLLs built from src/AI instead of the shipped ones" ON)
if(COS_USE_REBUILT_AI)
    add_custom_target(AI_Deploy ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory "$<TARGET_FILE_DIR:dmcr>/ai_rebuilt" "$<TARGET_FILE_DIR:dmcr>/ai"
        COMMENT "Deploying rebuilt AI DLLs to ai/"
        VERBATIM)
    add_dependencies(AI_Deploy dmcr ${AI_TARGETS})
endif()

add_custom_target (i18n ALL COMMENT "Building i18n messages.")
file (GLOB es_PO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/lang/*)
set(es_POT ${CMAKE_CURRENT_SOURCE_DIR}/emulationstation.pot)
# update the pot
MESSAGE("Building the pot file")
add_custom_command (TARGET i18n COMMAND find "${CMAKE_SOURCE_DIR}/es-app" "${CMAKE_SOURCE_DIR}/es-core" -name "*.cpp" -o -name "*.h" | ${XGETTEXT_EXECUTABLE} -f - -o ${es_POT} --no-location --keyword=_)

foreach (es_PO_INPUT ${es_PO_FILES})
        get_filename_component (es_PO_LANG ${es_PO_INPUT} NAME_WE)
	MESSAGE("LANG = ${es_PO_LANG}")
	set (es_MO_OUTPUT ${es_PO_INPUT}/LC_MESSAGES/emulationstation.mo)
	# update the po from the pot
	add_custom_command (TARGET i18n COMMAND basename ${es_PO_INPUT} && ${MSGMERGE_EXECUTABLE} -U --no-fuzzy-matching ${es_PO_INPUT}/LC_MESSAGES/emulationstation.po ${es_POT})
	# compile the po to mo
        add_custom_command (TARGET i18n COMMAND ${MSGFMT_EXECUTABLE} -o ${es_MO_OUTPUT} ${es_PO_INPUT}/LC_MESSAGES/emulationstation.po --statistics)
        install (FILES ${es_MO_OUTPUT} DESTINATION share/locale/${es_PO_LANG}/LC_MESSAGES RENAME emulationstation.mo)
endforeach (es_PO_INPUT ${es_PO_FILES})

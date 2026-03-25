# Créer un executable à partir de fichiers sources
add_executable(test_vector test_vector.cpp)
# Fait le lien avec la bibliothèque créée précedemment et gtest
target_link_libraries(test_vector particules gtest_main)

add_executable(test_univers test_univers.cpp)
# On lie l'exécutable à la bibliothèque 'particules' et gtest
target_link_libraries(test_univers particules gtest_main)

add_executable(test_particule test_particule.cpp)
target_link_libraries(test_particule particules gtest_main)

include(GoogleTest)
gtest_discover_tests(test_particule)
gtest_discover_tests(test_vector)
gtest_discover_tests(test_univers)
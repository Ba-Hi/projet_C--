# Créer un executable à partir de fichiers sources
add_executable(test_vector  test_vector.cpp)

# Fait le lien avec la bibliothèque créée précedemment
# Les entêtes sont automatiquement détectés.
target_link_libraries(test_vector particules)


add_executable(test_univers test_univers.cpp)

# On lie l'exécutable à la bibliothèque 'particules' (qui contient Univers.o, Vector.o, etc.)
target_link_libraries(test_univers particules)
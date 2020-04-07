#!/usr/bin/env python3

import numpy as np
import os
import argparse


def generate_graph(nb_vertices, nb_edges, prop_infected):
    # Création des relations
    assert nb_edges >= 0 and nb_edges<=(nb_vertices*(nb_vertices-1))/2
    possible_relations = [(i,j) for i in range(nb_vertices) for j in list(range(i+1,nb_vertices))]
    indices = np.sort(np.random.choice(np.arange(len(possible_relations)), replace=False, size=nb_edges))
    edges = [possible_relations[i] for i in indices]
    assert len(edges) == nb_edges

    # Sélection des sommets infectés
    assert prop_infected>=0 and prop_infected<=100
    nb_infected = int(prop_infected*nb_vertices/100)
    assert nb_infected>=0 and nb_infected<=nb_vertices

    matrice_adjacence = np.zeros((nb_vertices, nb_vertices), dtype=np.int)
    for edge in edges:
        matrice_adjacence[edge[0], edge[1]] = 1
        matrice_adjacence[edge[1], edge[0]] = 1

    is_file, copy_number = True, 0
    while is_file:
        filename = f"{nb_vertices}_{nb_edges}_{prop_infected}_{copy_number}.txt"
        is_file = os.path.isfile(f"./exemplaires/{filename}")
        copy_number += 1

    with open('./exemplaires/' + filename, 'w+') as f:
      f.write(str(nb_vertices) + " " + str(nb_infected) + "\n")
      for i in range(nb_vertices):
          for j in range(nb_vertices):
              f.write(str(matrice_adjacence[i, j]) + " ")
          f.write("\n")

    malades = np.sort(np.random.choice(np.arange(nb_vertices), replace=False, size=nb_infected))
    with open('./exemplaires/' + filename, 'a') as f:
      for i in malades:
          f.write(str(i) + " ")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-N", "--N", help="Taille de la population (nombre de sommets)")
    parser.add_argument("-r", "--relations", help="Nombre total de relations (nombre d'arrêtes)")
    parser.add_argument("-p", "--pourcentage", help="Pourcentage de la population malade")
    args = parser.parse_args()

    if not os.path.exists('./exemplaires'):
        os.makedirs('exemplaires')

    generate_graph(int(args.N), int(args.relations), int(args.pourcentage))

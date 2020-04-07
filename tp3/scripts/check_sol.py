################################################################################
#
# Ce script vérifie si votre solution est valide. C'est le script qui sera
# utilisé pour la correction, donc assurez-vous que la sortie de votre
# script tp.sh est compatible avec ce script-ci.
#
# Argument 1 : Path vers l'exemplaire
# Argument 2 : Path vers la solution de cet exemplaire
# Argument 3 : Paramètre k
#
# Exemple d'utilisation :
#
#   1. Vous exécutez votre algorithme avec tp.sh et vous envoyez son résultat
#      vers un fichier texte :
#
#      ./tp.sh -e ./exemplaires/10_3_25_0.txt -p > sol_10_3_25_0.txt
#
#   2. Vous vérifiez si votre solution est valide avec ce script-ci (où k=3 ici):
#
#      python3 check_sol.py ./exemplaires/10_3_25_0.txt sol_10_3_25_0.txt 3
#
################################################################################

import numpy as np
import pathlib
import sys


# Initial sanity checks
if (len(sys.argv) != 4):
    exit("ERREUR : Ce script de vérification de solution prend trois " + \
         "arguments en entrée.")
if (not pathlib.Path(sys.argv[1]).is_file()):
    exit("ERREUR : Fichier " + sys.argv[1] + " inexistant.")
if (not pathlib.Path(sys.argv[2]).is_file()):
    exit("ERREUR : Fichier " + sys.argv[2] + " inexistant.")


# Un sommet ayant k voisins infectés devient lui-même infecté
try:
    k = int(sys.argv[3])
except:
    exit("Le paramètre k doit être un entier")


# Chargement de la solution
with open(sys.argv[2], 'r') as file:
    sol = []
    full_sol = file.read().split('\n\n')
    # Parse seulement la dernière solution proposée
    for line in full_sol[-1].strip().split('\n'):
        edge = map(int, line.strip().split(' '))
        sol.append(tuple(edge))

# Chargement du graphe et des paramètres
with open(sys.argv[1], 'r') as file:
    for num_line, lines in enumerate(file.readlines()):
        if num_line == 0:
            nombre_sommets, nombre_infectes = map(
                int, lines.strip().split(' '))
            matrice_adjacence = np.zeros(
                (nombre_sommets, nombre_sommets), dtype=bool)
        elif num_line > 0 and num_line <= nombre_sommets:
            # Matrice d'adjacence
            matrice_adjacence[num_line-1] = np.fromiter(map(int, lines.strip().split(' ')), bool)
        else:
            # Sommets infectés
            set_infectes = set(map(int, lines.strip().split(' ')))
            # Sommets non infectés
            set_sains = set(range(nombre_sommets)).difference(set_infectes)


# Vérification des arêtes fournies et mise à jour de la matrice d'adjacence
for edge in sol:
    try:
        matrice_adjacence[edge]
    except:
        exit("ERREUR : Problème avec le format de la solution.")

    if not matrice_adjacence[edge]:
        exit(f"ERREUR : L'arête {edge} n'existe pas dans le graphe fourni.")
    else:
        i, j = edge
        matrice_adjacence[i, j] = False
        matrice_adjacence[j, i] = False


def forward(set_sains, set_infectes):
    """Simule une itération de contamination
    """

    # Flag d'atteinte du point fixe
    done = False

    # On mémorise les individus sains et infectés à l'itération précédente
    anciens_infectes = set_infectes.copy()
    anciens_sains = set_sains.copy()

    for sommet_sain in anciens_sains:
        nb_voisins_infectes = 0
        for node, is_neighbor in enumerate(matrice_adjacence[sommet_sain]):
            if is_neighbor and node in anciens_infectes:
                nb_voisins_infectes += 1
                if nb_voisins_infectes == k:
                    set_sains.remove(sommet_sain)
                    set_infectes.add(sommet_sain)
                    break

    if len(set_infectes) == len(anciens_infectes):
        done = True

    return set_sains, set_infectes, done


# Propagation
done = False
while not done:
    set_sains, set_infectes, done = forward(set_sains, set_infectes)
    # Si plus de la moitié de la population est infectée, la solution est invalide
    if len(set_infectes) > nombre_sommets / 2:
        exit(f'ERREUR : Plus de la moitié de la population est atteinte.')


print("Votre solution est valide et a une valeur de", len(sol))

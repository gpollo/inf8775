import numpy as np
import os

def create_partition():
    tailles_exemplaires = [1000, 3000, 10000, 30000, 100000, 300000, 1000000, 3000000, 6000000, 8000000]
    try:
        os.mkdir("exemplaires")
        for nb_notes in tailles_exemplaires:
            for i in range(1, 11):              
                with open(os.path.join("exemplaires", "ex_" + str(nb_notes) + "_" + str(i) + ".txt"), 'w') as fichier_exemplaire:
                    fichier_exemplaire.write(str(nb_notes) + "\n")
                    notes = np.random.randint(0, 24, size=nb_notes, dtype='int64')
                    fichier_exemplaire.write(' '.join([str(n) for n in notes]))
    except:
        print("Les exemplaires ont deja ete crees. Si vous voulez les creer a nouveau, supprimer le dossier exemplaires.")


if __name__ == '__main__':
    create_partition()

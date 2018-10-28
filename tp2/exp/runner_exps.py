# try:
#     import exp_tiempos_variando_alpha_k
# except:
#     print("ERROR: exp_tiempos_variando_alpha_k")
#
# print("----------------------1-----------------------")
#
# try:
#     import exp_pca_n_vs_sin_pca
# except:
#     print("ERROR: exp_pca_n_vs_sin_pca")
#
# print("--------------------2-------------------------")
#
# try:
#     import exp_distintas_normas_inverted
# except:
#     print("ERROR: exp_distintas_normas_inverted")
#
# print("--------------------3-------------------------")
#
# try:
#     import exp_distintas_normas
# except:
#     print("ERROR: exp_distintas_normas")
#
#
# print("----------------------4-----------------------")
#
# try:
#     import exp_training_entries_vs_accuracy_vs_tiempo
# except:
#     print("ERROR: exp_training_entries_vs_accuracy_vs_tiempo")

print("----------------------4-----------------------")

try:
    import exp_variando_k_knn_knninverted.py
except:
    print("ERROR: exp_variando_k_knn_knninverted.py")



print("----------------------5-----------------------")

try:
    import exp_training_entries_vs_accuracy_vs_tiempo.py
except:
    print("ERROR: exp_training_entries_vs_accuracy_vs_tiempo.py")

import subprocess
subprocess.check_output("shutdown -h now", stderr=subprocess.STDOUT, shell=True).decode("utf-8")






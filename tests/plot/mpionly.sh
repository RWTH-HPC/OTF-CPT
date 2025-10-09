# REQUIRES: plot-requirements
# RUN: rm -rf %t/out
# RUN: %plotscript --mpi-only -o %t/out %project_src_dir/script/data
# RUN: test -f %t/out/data_graph.pdf
# RUN: test -f %t/out/data_graph.png
# RUN: test -f %t/out/data_metrics.pdf
# RUN: test -f %t/out/data_metrics.png


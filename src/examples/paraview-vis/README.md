Insitu ParaView visualization using the Ascent Extract interface
================================================================

* Install Ascent with mfem turned on so that proxies/laghos is built
   - `git clone --recursive https://github.com/alpine-dav/ascent.git`
   - `cd ascent`
   - `python scripts/uberenv/uberenv.py --install --prefix="build" --spec='%gcc+mfem'`

   - `build/ascent-install` will contain the ascent installation and built examples
   - `build/spack` will contain the spack packages. Note: spack modules are NOT setup
   - execute `. ascent/build/spack/share/spack/setup-env.sh` to setup spack package manager
* Patch ParaView spack specification to install from master
   - `cd build/spack`
   - `patch -p1 < ~/projects/ascent/src/examples/paraview-vis/paraview-package.patch`
* Install ParaView (use older matplotlib because newest requires python3.)
   - `spack install paraview +python ^py-matplotlib@2.2.3 ~tk ^hdf5@1.8.21`
* Test with available simulations - VTK files and images will be generated in the curent directory.
   - Test `proxies/cloverleaf3d`
     - `cd build/ascent-install/examples/ascent/proxies/cloverleaf3d`
     - `ln -s ~/projects/ascent/src/examples/paraview-vis/paraview_ascent_source.py
        ln -s ~/projects/ascent/src/examples/paraview-vis/paraview-vis-cloverleaf3d.py paraview-vis.py;
        ln -s ~/projects/ascent/src/examples/paraview-vis/ascent_actions.json`
     - replace paraview_path from paraview-vis-xxx.py with the result of `$(spack location --install-dir paraview)` and scriptName with the correct path to 'paraview_ascent_source.py'
     - `$(spack location --install-dir mpi)/bin/mpiexec -n 2 cloverleaf3d_par > output.txt 2>&1`
     - examine the generated VTK files the images
   - Similarily test: `proxies/kripke`, `proxies/laghos`, `proxies/lulesh` (you need to create the links)
     - `$(spack location --install-dir mpi)/bin/mpiexec -np 8 lulesh_par -i 10 -s 32 > output.txt 2>&1`
     - `$(spack location --install-dir mpi)/bin/mpiexec -np 8 kripke_par --procs 2,2,2 --zones 32,32,32 --niter 5 --dir 1:2 --grp 1:1 --legendre 4 --quad 4:4 > output.txt 2>&1`
     - `$(spack location --install-dir mpi)/bin/mpiexec -n 8 ./laghos_mpi -p 1 -m data/cube01_hex.mesh -rs 2 -tf 0.6 -visit -pa`
   - Test noise from `examples/ascent/synthetic/noise`
     - `$(spack location --install-dir mpi)/bin/mpiexec -np 8 noise_par  --dims=32,32,32 --time_steps=5 --time_delta=1 > output.txt 2>&1`


TODO:
* Add installing ParaView + Ascent Support as an option when installing Ascent
* Add testing

NOTE:
Global extents are computed for uniform and rectilinear topologies but
they are not yet computed for a structured topology (lulesh). This
means that for lulesh and datasets that have a structured topology we
cannot save a correct parallel file that represents the whole dataset.



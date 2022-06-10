mkdir -p dir/dir1/{dir1.1,dir1.2,dir1.3}
mkdir -p dir/dir1/dir1.2/{dir1.2.1,dir1.2.2,dir1.2.3}
mkdir -p dir/dir2/{dir2.1,dir2.2}
mkdir -p dir/dir2/dir2.2/dir2.2.1
mkdir -p dir/dir2/dir2.2/{dir2.2.1,dir2.2.2}
mkdir -p dir/dir3/dir3.1
mkdir -p dir/dir4
mkdir -p dir/dir5

touch dir/dir1/dir1.1/file.scala
touch dir/dir1/dir1.2/file.scala
touch dir/dir2/dir2.2/{file.c,file.cpp}
touch dir/dir2/dir2.2/dir2.2.2/{file.go,file.rb}
touch dir/dir3/{file.js,file.java}
touch dir/dir3/dir3.1/{file.c,file.cpp}
> dir/dir4/file.py

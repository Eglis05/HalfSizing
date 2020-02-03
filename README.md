Well generally speaking the first thing to do is read all the data
from the TAG file. After we have read all the data we can start
changing it with the most important change being the division
of the width and height by 2. Then, of course, we need to change the
size of the data of the image. Then we need to use the half-sizing
algorithm to update the data of the image. In the end, we need to write
all the data in the new file.

The site I relied the most upon, from which I got a part of the struct
of the TAG file as it was a bit new to me, was:
https://gist.github.com/Loggie/3253731
I also read a bit to fill the gaps from this page:
http://netghost.narod.ru/gff/vendspec/tga/tga.txt

My half-sizing algorithm in general terms works by taking 4 pixels
and merging (averaging) them into one. The algorithm would be a bit more
understandable why the position and the old position are as written in the
code if you think of the data read as a matrix or a two-dimensional array.
Though we will have to do the conversion from the matrix position to the array
position by ourselves.
After reading one byte from all the 4 pixels we add all of them up and average
the sum and then save it back. We do this for all pixels of the new image.
(A bit more detailed comments inside the code for easier reference to which
part it addresses.)

The first problem was the half-sizing algorithm which was described above.
The first real problem was the addition of the pixels, as in the case that you convert it to
int the cases where the byte is more than 128 it will count as a negative number
in the conversion and everything will get messed up. I solved this by doing bit by bit
checking and addition in the sum for all the colors.
The second problem wasn't hard to fix but a bit difficult to locate. After halving the
size of the width I was using double its value instead of the oldwidth variable.
In case when halve the width wasn't an integer would mess the whole image but after
some debugging I was able to find this problem.
A space problem was that at the beginning I was creating newPixelData instead of
overwriting the old one. After noticing that I wasn't using the ones I was overwriting
I reduced the space needed by the algorithm. In case of space being the real problem,
we can trade it with speed by not reading the whole thing but jumping with seekg
and reading only the ones we need. Even though we read the whole thing we only need 4
byte memory to save the pixels as we can overwrite after using them to calculate the average.
And then we can directly write the average into the file without wasting space-saving it.
With this last idea, the speed would be worst than the first one so it might not be worth it
unless in some extreme cases.

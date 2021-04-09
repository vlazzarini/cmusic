
#!/bin/sh

find $1 \( -perm 755 -a ! -type d \) -print > /tmp/rmexe1$$
if test ! -s /tmp/rmexe1$$ 
then
	exit 0
fi
file `cat /tmp/rmexe1$$` | \
	sed -e "/shell/d" -e "/commands/d" -e "/text/d" -e "s/:.*//" > /tmp/rmexe2$$
rm -f `cat /tmp/rmexe2$$`
rm -f /tmp/rmexe1$$ /tmp/rmexe2$$

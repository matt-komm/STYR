SCRIPT_DIR=`dirname ${BASH_SOURCE[0]}`
SCRIPT_DIR=`readlink -e $SCRIPT_DIR`


LOGFILE=$SCRIPT_DIR/delphes.log
VARFILE=$SCRIPT_DIR/delphes.vars
date > $LOGFILE
echo "=== installing delphes ===" >> $LOGFILE

function execute()
{
    echo -n "  downloading ... "
    wget https://github.com/delphes/delphes/archive/3.4.2pre15.tar.gz &>> $LOGFILE || return 1
    echo "ok"
    echo -n "  extracting ... "
    tar -xvzf 3.4.2pre15.tar.gz &>> $LOGFILE || return 1
    echo "ok"
    echo -n "  configuring ... "
    cd delphes-3.4.2pre15 &>> $LOGFILE
    rm cmake/FindPythia8.cmake &>> $LOGFILE
    INSTALLDIR=$SCRIPT_DIR/delphes-3.4.2pre15/release
    mkdir build &>> $LOGFILE
    cd build &>> $LOGFILE
    cmake .. \
        -DCMAKE_INSTALL_PREFIX=$INSTALLDIR \
        -DPYTHIA8_FOUND="" \
        -DCMAKE_BUILD_TYPE=Release &>> $LOGFILE || return 1 
    echo "ok"
    echo -n "  building ... "
    make -j4 &>> $LOGFILE || return 1
    echo "ok"
    echo -n "  installing ... "
    make install &>> $LOGFILE || return 1
    echo "ok"
    
    PATHVAR=$INSTALLDIR/bin
    LIBVAR=$INSTALLDIR/lib
    DELPHESVAR=$INSTALLDIR
    return 0
}

echo "=== installing delphes ==="
if execute
then
    echo "=== delphes installed ==="
    echo "PATH="$PATHVAR":\$PATH" > $VARFILE
    echo "LD_LIBRARY_PATH="$LIBVAR":\$LD_LIBRARY_PATH" >> $VARFILE
    echo "DELPHES_DIR="$DELPHESVAR >> $VARFILE
else
    echo "failed"
    echo "  ... (last 200 lines from log file) ..."
    tail -n 200 $LOGFILE
fi
    

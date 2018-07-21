SCRIPT_DIR=`dirname ${BASH_SOURCE[0]}`

PATHVAR=""
LIBVAR=""
PYTHONVAR=""

LOGFILE=$SCRIPT_DIR/delphes.log
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
    INSTALLDIR=$PWD/release
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
    return 0
}

echo "=== installing delphes ==="
if execute
    then
    echo "=== delphes installed ==="
    else
    echo "failed"
    echo "  ... (last 200 lines from log file) ..."
    tail -n 200 $LOGFILE
    fi
    

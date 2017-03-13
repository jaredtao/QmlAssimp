#copy dirs
#$(COPY_DIR) 命令是qmake生成的makefile中提供
CONFIG(autoDeploymentDir) {
    for(dir, deployDirs) {
        isEmpty(copyDirs.commands) {
            copyDirs.commands =$(COPY_DIR) $${dir} $${OUT_PWD}/$$basename(dir)
        } else {
            copyDirs.commands +=| $(COPY_DIR) $${dir} $${OUT_PWD}/$$basename(dir)
        }DESTDIR
    }
    win32{
        copyDirs.commands ~= s,/,\\\\,g
    }
    QMAKE_EXTRA_TARGETS +=copyDirs
    PRE_TARGETDEPS +=copyDirs
}

#copy files
#$(COPY) 命令是qmake生成的makefile中提供
CONFIG(autoDeploymentFiles) {
    for(file, deployFiles) {
        isEmpty(copyFiles.commands) {
            copyFiles.commands =$(COPY) $${file} $${OUT_PWD}
        } else {
            copyFiles.commands +=| $(COPY) $${file} $${OUT_PWD}
        }DESTDIR
    }
    win32{
        copyFiles.commands ~= s,/,\\\\,g
    }
    QMAKE_EXTRA_TARGETS +=copyFiles
    PRE_TARGETDEPS +=copyFiles
}

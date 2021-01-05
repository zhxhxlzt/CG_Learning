from pathlib import Path
import re
import sys

def Print(title, *argv):
    print(title, '=====>', *argv)

def ReadLines(filename: str):
    for encode in ['gbk', 'utf-8']:
        try:
            with open(filename, 'r', encoding=encode) as fp:
                return fp.readlines()
        except:
            continue

class FuncInfo:
    def __init__(self):
        self.funcName = None

class ClassInfo:
    def __init__(self):
        self.className = None
        self.lFuncInfo = []

    def AddFuncInfo(self, info):
        self.lFuncInfo.append(info)

    def Save(self):
        lines = []
        s = "void %s::RegisterAllMethod() {" % self.className
        lines.append(s)
        prefix = '\t'
        for funcInfo in self.lFuncInfo:
            s = prefix + "REGISTER_FUNC(%s);" % funcInfo.funcName
            lines.append(s)
        lines.append('}')
        text = '\n'.join(lines)
        return text

class HeadFileInfo:
    def __init__(self):
        self.filePath = ''
        self.fileName = ''
        self.dClassInfo = {}
        self.curClass = None

    def Load(self, filePath):
        self.filePath = filePath
        self.fileName = Path(filePath).name
        lines = ReadLines(filePath)
        for line in lines:
            self.UpdateClassInfo(line)
            if not self.curClass:
                continue
            funcName = self.GetFuncName(line)
            if funcName:
                funcInfo = FuncInfo()
                funcInfo.funcName = funcName
                self.curClass.AddFuncInfo(funcInfo)

    def Save(self):
        assert self.fileName, '文件名为空'
        lines = []
        s = '#include "%s" ' % self.fileName
        lines.append(s)
        for classInfo in self.dClassInfo.values():
            s = classInfo.Save()
            lines.append(s)

        if len(lines) == 1:
            return ''
        text = '\n'.join(lines)
        return text


    def UpdateClassInfo(self, line: str):
        line = line.strip('\n')
        ret = re.search(".*META_OBJECT\((?P<className>\w+).*\)", line)
        # 有 ";" 说明没有定义函数体，才需要自动生成
        if ret:
            className = ret.group("className")
            print('class line:', line)
            if line[-1] == ';':
                if self.curClass is None or self.curClass.className != className:
                    self.curClass = ClassInfo()
                    self.curClass.className = className
                    self.dClassInfo[className] = self.curClass
                    return True
            else:
                Print('手动处理的类:', className)
        return False

    def GetFuncName(self, line: str):
        ret = re.search("META_.*FUNC.*\((?P<params>.*)\)", line)
        if ret:
            params = ret.group("params")
            funcName = params.split(",")[0].strip()
            return funcName


class RegisterFuncMgr:
    def __init__(self):
        self.lHeadFileInfo = []
        self.lFilters = ["MetaObject.h"]

    def Load(self, rootPath):
        self.lHeadFileInfo.clear()
        for headFilePath in Path(rootPath).rglob("*.h"):
            if self.CheckInFilters(headFilePath.name):
                Print('filtered head file:', headFilePath)
                continue
            Print('cur head file:', headFilePath.as_posix())
            headInfo = HeadFileInfo()
            headInfo.Load(headFilePath)
            self.lHeadFileInfo.append(headInfo)

    def Save(self, filePath):
        filePath = Path(filePath)
        lines = []
        for headFileInfo in self.lHeadFileInfo:
            s = headFileInfo.Save()
            lines.append(s)

        text = '\n'.join(lines)

        if filePath.exists():
            data = ''.join(ReadLines(filePath))
            if text == data:
                Print('不需要更新')
                return

        with open(filePath, 'w') as fp:
            fp.write(text)

    def CheckInFilters(self, fileName):
        for f in self.lFilters:
            if fileName == f:
                return True
        return False


if __name__ == '__main__':
    mgr = RegisterFuncMgr()
    Print('sys.argv:', sys.argv)
    _, filePath, savePath = sys.argv
    mgr.Load(filePath)
    mgr.Save(savePath)



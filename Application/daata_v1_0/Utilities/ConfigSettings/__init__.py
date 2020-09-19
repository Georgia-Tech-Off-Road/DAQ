import configparser as cp





class ConfigSettings:
    def __init__(self):
        self.parser = cp.ConfigParser()
        self.path = 'settings.ini'
        self.setSettings()
        self.writeSettings()
        # print(self.parser['DEFAULT']['path'])
        # self.parser.read(self.path)
        self.printSettings()

    def setSettings(self):
        self.parser.add_section('Network Drive')
        self.parser.set('Network Drive', 'drive', 'G:')

    def printSettings(self):
        for section_name in self.parser.sections():
            print('Section:', section_name)
            print('Options:', self.parser.options(section_name))
            for name, value in self.parser.items(section_name):
                print('  %s = %s' % (name, value))

    def writeSettings(self):
        self.parser.write(self.path)

ConfigSettings()
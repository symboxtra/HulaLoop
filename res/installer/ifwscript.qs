function Component()
{
//    console.log("Component" + component.enabled);
}

Component.prototype.createOperations = function()
{
//    QMessageBox["information"]("os.information", "Installer", "Test message!", QMessageBox.Ok);
    console.log("BEGIN");
    component.createOperations();
    console.log("CREATED");

    if (systemInfo.productType === "windows")
    {
        console.log("TEST");
        component.addOperation("CreateShortcut", "@TargetDir@/hulaloop-uninstaller.exe", "@DesktopDir@/HulaLoop Uninstaller.lnk");
//        component.addOperation("CreateShortcut", "@TargetDir@/bin/hulaloop-launcher.exe", "@StartMenuDir@/HulaLoop.lnk",
//            "workingDirectory=@TargetDir@", "iconPath=%SystemRoot%/system32/SHELL32.dll", "iconId=2", "description=Launch HulaLoop");
//        component.addOperation("CreateShortcut", "@TargetDir@/hulaloop-uninstaller.exe", "@StartMenuDir@/HulaLoop Uninstaller.lnk",
//            "workingDirectory=@TargetDir@", "iconPath=%SystemRoot%/system32/SHELL32.dll", "iconId=2", "description=Uninstall HulaLoop");
    }
}

function Controller()
{
    console.log("Controller");
    if (installer.isUninstaller())
    {
        installer.setDefaultPageVisible(QInstaller.Introduction, false);
        installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
        installer.setDefaultPageVisible(QInstaller.LicenseCheck, false);
    }
    console.log("end");
}

//Controller.prototype.IntroductionPageCallback() = function() {}
//Controller.prototype.TargetDirectoryPageCallback() = function() {}
//Controller.prototype.ComponentSelectionPageCallback() = function() {}
//Controller.prototype.StartMenuDirectoryPageCallback() = function() {}
//Controller.prototype.ReadyForInstallationPageCallback() = function() {}
//Controller.prototype.PerformInstallationPageCallback() = function() {}
//Controller.prototype.FinishedPageCallback() = function() {}


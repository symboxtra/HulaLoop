####################################################################################
#
# Generate and merge coverage for each test executable individually since
# OpenCppCoverage does not accept more than one exectuable.
#
# This is in place of ctest -V and will run all executables located in the build/bin/test folder.
# IMPORTANT: If you add a test outside that folder (like TestGUI), you need to modify this.
#
# A better alternative to this script might be something with ctest_coverage()
# This works for now until we have some time to look into it.
# Reference: https://cmake.org/cmake/help/v3.12/manual/ctest.1.html#ctest-coverage-step
#
# Reference: https://github.com/OpenCppCoverage/OpenCppCoverage/wiki#merging-coverage---input_coverage----export_typebinary
#
####################################################################################

echo $MyInvocation.MyCommand.Name

Write-Host "--------------------------------------------------" -Foreground yellow
Write-Host "      Generate Coverage w/ OpenCppCoverage"         -Foreground yellow
Write-Host "--------------------------------------------------" -Foreground yellow

# First argument is the prefix path to the build folder
if ($args[0])
{
    $prefix = $args[0]
}
else
{
    $prefix = "build"
    $command = $MyInvocation.MyCommand.Name
    Write-Host "Using default prefix '${prefix}'." -Foreground blue
    Write-Host "Use: ${command} myPrefix to change." -Foreground blue
    echo ""
}

# Make sure it exists
if (-Not (Test-Path "${prefix}/bin/test"))
{
    Write-Host "${prefix}/bin/test does not exist." -Foreground blue
    Write-Host "Please try again with a directory containing a valid build." -Foreground blue
    echo ""
    exit 1
}

# Run TestGUI with all Qt modules excluded via q*
echo ""
Write-Host "Begin TestGUI.exe..." -Foreground yellow
Write-Host "--------------------------------------" -Foreground yellow
echo ""

$previous = "${prefix}/TestGUI.cov"
OpenCppCoverage --excluded_modules=q* --modules $PWD --export_type="binary:$previous" "${prefix}/bin/TestGUI.exe"
if ($LastExitCode -ne 0) {$host.SetShouldExit($LastExitCode)}

echo ""
Write-Host "--------------------------------------" -Foreground yellow
echo ""

$files = @(Get-Item "${prefix}/bin/test/*.exe")
foreach ($file in $files)
{
    $filename = Split-Path "$file" -leaf
    $current = "${prefix}/${filename}.cov"

    echo ""
    Write-Host "Begin ${filename}..." -Foreground yellow
    Write-Host "--------------------------------------" -Foreground yellow
    echo ""

    # Run the current test executable plus the input of the previous
    # This is basically recursively collecting the coverage
    OpenCppCoverage --modules $PWD --input_coverage="$previous" --export_type="binary:${prefix}/${filename}.cov" "$file"
    if ($LastExitCode -ne 0) {$host.SetShouldExit($LastExitCode)}
    $previous = "${prefix}/${filename}.cov"

    echo ""
    Write-Host "End of ${filename}" -Foreground yellow
    Write-Host "--------------------------------------" -Foreground yellow
    echo ""
}

Write-Host "Merging coverage for upload..." -Foreground yellow
Write-Host "--------------------------------------" -Foreground yellow

# Merge the coverage files to a final report
OpenCppCoverage --modules $PWD --input_coverage="$previous" --export_type=cobertura:"${prefix}/cobertura.xml"
echo ""

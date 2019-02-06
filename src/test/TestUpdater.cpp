#include <gtest/gtest.h>

#include <QApplication>
#include <QDir>
#include <QFile>

#include "Updater.h"

using namespace hula;

class TestUpdater : public ::testing::Test {

    protected:
        QApplication *app = nullptr;
        Updater *updater = nullptr;

        virtual void SetUp()
        {
            int argc = 0;
            char **argv = nullptr;

            app = new QApplication(argc, argv);
            updater = new Updater();
        }

        virtual void TearDown()
        {
            app->exit();
            delete updater;
            delete app;
        }

};

TEST_F(TestUpdater, checkAndDownloadUpdate)
{
    updater->setUpdateHost("https://api.github.com/repos/symboxtra/HulaLoop/releases/latest");
    ASSERT_EQ(updater->checkForUpdate(), 1);
    ASSERT_EQ(updater->downloadUpdate(), 1);

    QFile file(QDir::tempPath() + "/" + updater->getDownloadFileName());
    ASSERT_TRUE(file.exists());
    ASSERT_EQ(file.size(), updater->getDownloadSize());
}

TEST_F(TestUpdater, checkForUpdateError)
{
    updater->setUpdateHost("https://google.com");
    ASSERT_EQ(updater->checkForUpdate(), 0);
}

TEST_F(TestUpdater, parseTag1)
{
    QList<int> versionParts = updater->parseTagName("0.0.0.0b");
    for (int i = 0; i < versionParts.size(); i++)
    {
        ASSERT_TRUE(versionParts.at(i) == 0);
    }
}

TEST_F(TestUpdater, parseTag2)
{
    QList<int> versionParts = updater->parseTagName("0.0.0");
    ASSERT_TRUE(versionParts.at(0) == 0);
    ASSERT_TRUE(versionParts.at(1) == 0);
    ASSERT_TRUE(versionParts.at(2) == 0);
}

TEST_F(TestUpdater, parseTag3)
{
    QList<int> versionParts = updater->parseTagName("0a.0.0.0");
    for (int i = 0; i < versionParts.size(); i++)
    {
        ASSERT_TRUE(versionParts.at(i) == 0);
    }
}

TEST_F(TestUpdater, parseTag4)
{
    QList<int> versionParts = updater->parseTagName("0000");
    for (int i = 0; i < versionParts.size(); i++)
    {
        ASSERT_TRUE(versionParts.at(i) == -1);
    }
}

TEST_F(TestUpdater, parseTag5)
{
    QList<int> versionParts = updater->parseTagName("0..0.0");
    ASSERT_TRUE(versionParts.at(0) == 0);
    ASSERT_TRUE(versionParts.at(1) == 0);
    ASSERT_TRUE(versionParts.at(2) == 0);
}

TEST_F(TestUpdater, parseTag6)
{
    QList<int> versionParts = updater->parseTagName("");
    for (int i = 0; i < versionParts.size(); i++)
    {
        ASSERT_TRUE(versionParts.at(i) == -1);
    }
}

TEST_F(TestUpdater, parseTag7)
{
    QList<int> versionParts = updater->parseTagName("a.b.c.d");
    for (int i = 0; i < versionParts.size(); i++)
    {
        ASSERT_TRUE(versionParts.at(i) == -1);
    }
}

TEST_F(TestUpdater, parseTag8)
{
    QList<int> versionParts = updater->parseTagName("0.1.0.123");
    ASSERT_TRUE(versionParts.at(0) == 0);
    ASSERT_TRUE(versionParts.at(1) == 1);
    ASSERT_TRUE(versionParts.at(2) == 0);
}
#include <gtest/gtest.h>

#include <QCoreApplication>
#include <vector>

#include "CLIArgs.h"

#define DEFAULT_ARGS 1
#define LONG_OPT "--"
#define SHORT_OPT "-"

#define OPT_TEST(name, ...)                           \
    std::vector<std::string> args{name, __VA_ARGS__}; \
                                                      \
    TestCLIArgs t(args);                              \
    bool success = t.parse();                         \
    HulaSettings *s = HulaSettings::getInstance();    \


class TestCLIArgs {
    public:

        QCoreApplication *app;
        HulaImmediateArgs extraArgs;
        int argc = 0;
        char **argv = NULL;

        TestCLIArgs(const std::vector<std::string> &args)
        {
            createArgv(args);
            this->app = new QCoreApplication(this->argc, this->argv);
        }

        /**
         * Create the standard char **argv from a vector of args.
         * The stored array will also contains any default arguments.
         *
         * @param args Arguments
         */
        void createArgv(const std::vector<std::string> &args)
        {
            // Delete any old args
            deleteArgv();

            // Create new char array
            argc = args.size() + DEFAULT_ARGS;
            argv = new char *[argc + 1];
            argv[argc] = NULL; // NULL terminator

            // Insert any default arguments
            // If you add something, be sure to update the DEFAULT_ARGS macro
            argv[0] = strdup("TestCLIArgs");

            for (size_t i = 0; i < args.size(); i++)
            {
                argv[i + DEFAULT_ARGS] = strdup(args[i].c_str());
            }
        }

        /**
         * Delete the allocated args and argv array.
         */
        void deleteArgv()
        {
            for (int i = 0; i < argc; i++)
            {
                free(argv[i]);
            }
            if (argv != NULL)
            {
                delete [] argv;
            }

            argc = 0;
            argv = NULL;
        }

        /**
         * Wrapper for the parseArgsQt function.
         */
        bool parse()
        {
            return parseArgsQt(*(this->app), this->extraArgs);
        }

        ~TestCLIArgs()
        {
            delete app;

            deleteArgv();
        }
};

/**
 * Set the output file short option.
 *
 * EXPECTED:
 *      output file matches in settings
 */
TEST(TestCLIArgs, short_opt_output_file)
{
    OPT_TEST(SHORT_OPT HL_OUT_FILE_SO, "test-file");

    EXPECT_EQ(t.extraArgs.outputFilePath, "test-file");
}

/**
 * Set the output file long option.
 *
 * EXPECTED:
 *      output file matches in settings
 */
TEST(TestCLIArgs, long_opt_output_file)
{
    OPT_TEST(LONG_OPT HL_OUT_FILE_LO, "test-file");

    EXPECT_EQ(t.extraArgs.outputFilePath, "test-file");
}

/************************************************************/

/**
 * Set the delay short option.
 *
 * EXPECTED:
 *      delay matches in settings
 */
TEST(TestCLIArgs, short_opt_delay)
{
    OPT_TEST(SHORT_OPT HL_DELAY_TIME_SO, "5.7");

    EXPECT_EQ(t.extraArgs.delay, "5.7");
}

/**
 * Set the delay long option.
 *
 * EXPECTED:
 *      delay matches in settings
 */
TEST(TestCLIArgs, long_opt_delay)
{
    OPT_TEST(LONG_OPT HL_DELAY_TIME_LO, "5.7");

    EXPECT_EQ(t.extraArgs.delay, "5.7");
}

/************************************************************/

/**
 * Set the record timer short option.
 *
 * EXPECTED:
 *      duration matches in settings
 */
TEST(TestCLIArgs, short_opt_duration)
{
    OPT_TEST(SHORT_OPT HL_RECORD_TIME_SO, "5.7");

    EXPECT_EQ(t.extraArgs.duration, "5.7");
}

/**
 * Set the record timer long option.
 *
 * EXPECTED:
 *      duration matches in settings
 */
TEST(TestCLIArgs, long_opt_duration)
{
    OPT_TEST(LONG_OPT HL_RECORD_TIME_LO, "5.7");

    EXPECT_EQ(t.extraArgs.duration, "5.7");
}

/************************************************************/

/**
 * Set the record trigger short option.
 *
 * EXPECTED:
 *      startRecord is true in extraArgs
 */
TEST(TestCLIArgs, short_opt_record)
{
    OPT_TEST(SHORT_OPT HL_TRIGGER_RECORD_SO);

    EXPECT_TRUE(t.extraArgs.startRecord);
}

/**
 * Set the record trigger long option.
 *
 * EXPECTED:
 *      startRecord is true in extraArgs
 */
TEST(TestCLIArgs, long_opt_record)
{
    OPT_TEST(LONG_OPT HL_TRIGGER_RECORD_LO);

    EXPECT_TRUE(t.extraArgs.startRecord);
}

/************************************************************/

/**
 * Set the sample rate short option.
 *
 * EXPECTED:
 *      sample rate matches in settings
 */
TEST(TestCLIArgs, short_opt_sample_rate)
{
    OPT_TEST(SHORT_OPT HL_SAMPLE_RATE_SO, "44100");

    EXPECT_EQ(s->getSampleRate(), 44100);
}

/**
 * Set the sample rate long option.
 *
 * EXPECTED:
 *      sample rate matches in settings
 */
TEST(TestCLIArgs, long_opt_sample_rate)
{
    OPT_TEST(LONG_OPT HL_SAMPLE_RATE_LO, "44100");

    EXPECT_EQ(s->getSampleRate(), 44100);
}

/**
 * Sample rate long opt with invalid number
 *
 * EXPECTED:
 *      parse returns false and prints error
 */
TEST(TestCLIArgs, NAN_arg_long_opt_sample_rate)
{
    OPT_TEST(LONG_OPT HL_SAMPLE_RATE_LO, "not_a_number");

    EXPECT_FALSE(success);
}

/************************************************************/

/**
 * Set the encoding short option.
 *
 * EXPECTED:
 *      encoding matches in settings
 */
TEST(TestCLIArgs, short_opt_encoding)
{
    OPT_TEST(SHORT_OPT HL_ENCODING_SO, "WAV");

    EXPECT_EQ(s->getOutputFileEncoding(), WAV);
}

/**
 * Set the encoding long option.
 *
 * EXPECTED:
 *      encoding matches in settings
 */
TEST(TestCLIArgs, long_opt_encoding)
{
    OPT_TEST(LONG_OPT HL_ENCODING_LO, "WAV");

    EXPECT_EQ(s->getOutputFileEncoding(), WAV);
}

/**
 * Encoding long opt with invalid format
 *
 * EXPECTED:
 *      parse returns false and prints error
 */
TEST(TestCLIArgs, NAN_arg_long_opt_encoding)
{
    OPT_TEST(LONG_OPT HL_ENCODING_LO, "not_a_format");

    EXPECT_FALSE(success);
}

/************************************************************/

/**
 * Set the input device short option.
 *
 * EXPECTED:
 *      input device matches in settings
 */
TEST(TestCLIArgs, short_opt_input_device)
{
    OPT_TEST(SHORT_OPT HL_INPUT_DEVICE_SO, "Device");

    EXPECT_EQ(t.extraArgs.inputDevice, "Device");
}

/**
 * Set the input device long option.
 *
 * EXPECTED:
 *      input device matches in settings
 */
TEST(TestCLIArgs, long_opt_input_device)
{
    OPT_TEST(LONG_OPT HL_INPUT_DEVICE_LO, "Device");

    EXPECT_EQ(t.extraArgs.inputDevice, "Device");
}

/************************************************************/

/**
 * Set the output device short option.
 *
 * EXPECTED:
 *      output device matches in settings
 */
TEST(TestCLIArgs, short_opt_output_device)
{
    OPT_TEST(SHORT_OPT HL_OUTPUT_DEVICE_SO, "Device");

    EXPECT_EQ(t.extraArgs.outputDevice, "Device");
}

/**
 * Set the output device long option.
 *
 * EXPECTED:
 *      output device matches in settings
 */
TEST(TestCLIArgs, long_opt_output_device)
{
    OPT_TEST(LONG_OPT HL_OUTPUT_DEVICE_LO, "Device");

    EXPECT_EQ(t.extraArgs.outputDevice, "Device");
}

// TODO: Non-existent device catch

/************************************************************/

/**
 * Set the list devices short option.
 *
 * EXPECTED:
 *      devices are listed
 */
TEST(TestCLIArgs, short_opt_list_device)
{
    OPT_TEST(SHORT_OPT HL_LIST_DEVICES_SO);
}

/**
 * Set the list devices long option.
 *
 * EXPECTED:
 *      devices are listed
 */
TEST(TestCLIArgs, long_opt_list_device)
{
    OPT_TEST(LONG_OPT HL_LIST_DEVICES_LO);
}

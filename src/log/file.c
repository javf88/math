/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include "file.h"
#include "levels.h"

/******************************************************************************/
/*    PRIVATE DATA                                                            */
/******************************************************************************/

#define MAX_NAME_LEN  (50U)

#define FORCED_INIT_VAL     NULL

/******************************************************************************/
/*    IMPLEMENTATION                                                          */
/******************************************************************************/

void constructor(void)
{
    init(LOG_TO_FILE);
    LOG_DEBUG("Calling constructor().");

    return;
}

void destructor(void)
{
    LOG *file = get();
    LOG_DEBUG("Calling destructor().");

    file++;
    if (file->name != NULL)
    {
        LOG_INFO("Closing file %s", file->name);
        free(file->name);
        file->name = NULL;
    }

    if (file->descriptor != NULL)
    {
        LOG_INFO("Flushing file %p", file->descriptor);
        fflush(file->descriptor);
        fclose(file->descriptor);
        file->descriptor = NULL;
    }

    LOG_DEBUG("Freeing \"static LOG *files\".");
    return;
}

LOG* get()
{
    /* Only the last {NULL, NULL} is a sentinel,
     * the second for the actual LOG* file */
    const LOG initial[3U] = {{stderr, "stderr"}, {NULL, NULL}, {NULL, NULL}};

    static LOG *files = NULL;
    if (files == NULL)
    {
        files = malloc(sizeof(LOG) * 3U);
        memcpy(files, initial, sizeof(LOG) * 3U);
    }

    return files;
}

STATIC LOG* set(LOG *newFile)
{
    /* file is "static LOG *files" */
    LOG *files, *tmp;
    tmp = files = get();
    tmp++;

    if (newFile == FORCED_INIT_VAL)
    {
        tmp->descriptor = FORCED_INIT_VAL;
        tmp->name = FORCED_INIT_VAL;
    }
    else
    {
        if (tmp->descriptor == FORCED_INIT_VAL)
        {
            memcpy(tmp, newFile, sizeof(LOG));
        }
        else
        {
            LOG_WARNING("File %s:%p was not set.", newFile->name, newFile->descriptor);
        }
    }

    return files;
}

STATIC int32_t make(const char *dir)
{
    /* Forcing initial state */
    errno = 0;
    /* Creating rel dir */
    mkdir(dir, 0777);
    switch (errno)
    {
        case 0:
            LOG_INFO("Directory %s was created. [CODE %d]", dir, errno);
            break;
        case EEXIST:
            LOG_WARNING("Directory %s exists already. [CODE %d]", dir, errno);
            break;
        case ENOENT:
        default:
            /* If dir is not created, opening file routine cleans up */
            LOG_WARNING("Directory %s was not created! [CODE %d]", dir, errno);
            break;
    }

    return errno;
}

STATIC char* make_name(const char *nameFormat)
{
    char *name = malloc(sizeof(char) * MAX_STR_LEN);
    if (name == NULL)
    {
        LOG_WARNING("Filename was not created.");
    }
    else
    {
        time_t secs;
        /* Creating tmp name */
        time(&secs);
        sprintf(name, nameFormat, secs);
        LOG_INFO("Name %s was made.", name);
    }

    return name;
}

STATIC LOG* open(char *filename)
{
    LOG *file = malloc(sizeof(LOG));
    file->descriptor = fopen(filename, "w");
    if (file->descriptor == NULL)
    {
        LOG_WARNING("File %s was not created! [CODE %d]", file->name, errno);
        free(file);
        file = NULL;
    }
    else
    {
        file->name = filename;
        LOG_INFO("File %s:%p was created.", file->name, file->descriptor);
    }

    return file;
}

STATIC LOG* init(uint32_t toFileFlag)
{
    LOG *files = get();
    if (toFileFlag == 0U)
    {
        LOG_INFO("Logging only into stderr.");
    }
    else
    {
        make("tmp");
        char *name = make_name("tmp/%ld.log");
        if (name != NULL)
        {
            LOG *newFile = open(name);
            /* Copying newFile into files[1U] entry(initial variable) */
            files = set(newFile);
            LOG_INFO("Logging into stderr and %s file.", files[1U].name);

            free(newFile);
        }
    }

    return files;
}

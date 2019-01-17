#include "databaseHelper.h"

//constructors
databaseHelper::databaseHelper() {

}

databaseHelper::databaseHelper(bool flag)
{
    createDatabase(flag);
}

//create the database
void databaseHelper::createDatabase(bool flag)
{
    /*
     * flag == true
     * drop tables
     *
     * flag == false
     * use existing data
     */

    if(flag) {
        //drop tables
    }

}

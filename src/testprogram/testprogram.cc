
 #include "../private.h"
 #include <udjat/module.h>
 #include <udjat/tools/logger.h>

 using namespace std;
 using namespace Udjat;

//---[ Implement ]------------------------------------------------------------------------------------------

int main(int argc, char **argv) {

	Logger::redirect();

	udjat_module_init(NULL);


	return 0;
}

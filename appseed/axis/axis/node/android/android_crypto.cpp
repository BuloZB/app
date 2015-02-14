//#include "framework.h"
//#include "android.h"


namespace android
{


   crypto::crypto(::aura::application * papp) :
      ::object(papp),
      ::crypto::crypto(papp)
   {
   }


   crypto::~crypto()
   {
   }


   string crypto::get_crypt_key_file_path()
   {

      return System.dir().path(getenv("home"), ".ca2/cryptkey");

   }


   //bool crypto::decrypt(primitive::memory & storageDecrypt, const primitive::memory & storageEncrypt, const char * pszSalt)
   //{

   //   ::primitive::memory memOut;

   //   ::primitive::memory memIn;

   //   memIn.assign(storageEncrypt.get_data(), storageEncrypt.get_size());

   //   if(!::crypto_decrypt(memOut, memIn, pszSalt))
   //      return false;


   //   storageDecrypt = memOut;

   //   return true;

   //}

   //bool crypto::encrypt(primitive::memory & storageEncrypt, const primitive::memory & storageDecrypt, const char * pszSalt)
   //{

   //   ::primitive::memory memOut;

   //   ::primitive::memory memIn;

   //   memIn.assign(storageDecrypt.get_data(), storageDecrypt.get_size());

   //   if(!::crypto_encrypt(memOut, memIn, pszSalt))
   //      return false;


   //   storageEncrypt = memOut;

   //   return true;

   //}


} // namespace android



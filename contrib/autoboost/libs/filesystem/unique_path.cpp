//  filesystem unique_path.cpp  --------------------------------------------------------//

//  Copyright Beman Dawes 2010

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  Library home page: http://www.boost.org/libs/filesystem

//--------------------------------------------------------------------------------------//

// define AUTOBOOST_FILESYSTEM_SOURCE so that <autoboost/filesystem/config.hpp> knows
// the library is being built (possibly exporting rather than importing code)
#define AUTOBOOST_FILESYSTEM_SOURCE

#ifndef AUTOBOOST_SYSTEM_NO_DEPRECATED
# define AUTOBOOST_SYSTEM_NO_DEPRECATED
#endif

#include <autoboost/filesystem/operations.hpp>
#include <cassert>

# ifdef AUTOBOOST_POSIX_API
#   include <fcntl.h>
#   ifdef AUTOBOOST_HAS_UNISTD_H
#      include <unistd.h>
#   endif
# else // AUTOBOOST_WINDOWS_API
#   include <windows.h>
#   include <wincrypt.h>
#   pragma comment(lib, "Advapi32.lib")
# endif

namespace {

void fail(int err, autoboost::system::error_code* ec)
{
  if (ec == 0)
    AUTOBOOST_FILESYSTEM_THROW( autoboost::system::system_error(err,
      autoboost::system::system_category(),
      "autoboost::filesystem::unique_path"));

  ec->assign(err, autoboost::system::system_category());
  return;
}

#ifdef AUTOBOOST_WINDOWS_API

int acquire_crypt_handle(HCRYPTPROV& handle)
{
  if (::CryptAcquireContextW(&handle, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT))
    return 0;

  int errval = ::GetLastError();
  if (errval != NTE_BAD_KEYSET)
    return errval;

  if (::CryptAcquireContextW(&handle, 0, 0, PROV_RSA_FULL, CRYPT_NEWKEYSET | CRYPT_VERIFYCONTEXT | CRYPT_SILENT))
    return 0;

  errval = ::GetLastError();
  // Another thread could have attempted to create the keyset at the same time.
  if (errval != NTE_EXISTS)
    return errval;

  if (::CryptAcquireContextW(&handle, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT))
    return 0;

  return ::GetLastError();
}

#endif

void system_crypt_random(void* buf, std::size_t len, autoboost::system::error_code* ec)
{
# ifdef AUTOBOOST_POSIX_API

  int file = open("/dev/urandom", O_RDONLY);
  if (file == -1)
  {
    file = open("/dev/random", O_RDONLY);
    if (file == -1)
    {
      fail(errno, ec);
      return;
    }
  }

  size_t bytes_read = 0;
  while (bytes_read < len)
  {
    ssize_t n = read(file, buf, len - bytes_read);
    if (n == -1)
    {
      close(file);
      fail(errno, ec);
      return;
    }
    bytes_read += n;
    buf = static_cast<char*>(buf) + n;
  }

  close(file);

# else // AUTOBOOST_WINDOWS_API

  HCRYPTPROV handle;
  int errval = acquire_crypt_handle(handle);

  if (!errval)
  {
    if (!::CryptGenRandom(handle, (DWORD)len, static_cast<unsigned char*>(buf)))
      errval = ::GetLastError();
    ::CryptReleaseContext(handle, 0);
  }

  if (!errval) return;

  fail(errval, ec);
# endif
}

}  // unnamed namespace

namespace autoboost { namespace filesystem { namespace detail {

AUTOBOOST_FILESYSTEM_DECL
path unique_path(const path& model, system::error_code* ec)
{
  std::wstring s (model.wstring());  // std::string ng for MBCS encoded POSIX
  const wchar_t hex[] = L"0123456789abcdef";
  char ran[] = "123456789abcdef";  // init to avoid clang static analyzer message
                                   // see ticket #8954
  assert(sizeof(ran) == 16);
  const int max_nibbles = 2 * sizeof(ran);   // 4-bits per nibble

  int nibbles_used = max_nibbles;
  for(std::wstring::size_type i=0; i < s.size(); ++i)
  {
    if (s[i] == L'%')                        // digit request
    {
      if (nibbles_used == max_nibbles)
      {
        system_crypt_random(ran, sizeof(ran), ec);
        if (ec != 0 && *ec)
          return "";
        nibbles_used = 0;
      }
      int c = ran[nibbles_used/2];
      c >>= 4 * (nibbles_used++ & 1);  // if odd, shift right 1 nibble
      s[i] = hex[c & 0xf];             // convert to hex digit and replace
    }
  }

  if (ec != 0) ec->clear();

  return s;
}

}}}



#include<cstdint>







namespace creation {

[[refl::override]]
void FreeFunction(void*);

[[refl::override]]
void* g_pSomething;

class BSSystemFile {
 public:
  enum AccessMode {
    kReadonly,
    kReadWrite,
    kWriteonly,
  };

  enum OpenMode { kNone, kAppend, kCreate, kTruncate, kExclude };

  enum Error {
    kSuccess,
    kInvalidFile,
    kInvalidPath,
    kFileError,
    kInvalid,
    kMemoryError,
    kBusy,
    kInvalidParam,
    kUnsupported
  };

  BSSystemFile();
  BSSystemFile(BSSystemFile& aRhs);
  BSSystemFile(BSSystemFile&& aRhs);
  
  explicit BSSystemFile(const char* name, AccessMode amode, OpenMode omode, bool async, uint32_t sizehint, bool unk8);
  ~BSSystemFile();

  
  BSSystemFile& operator=(BSSystemFile& aRhs);
  BSSystemFile& operator=(BSSystemFile&& aRhs);

  void Close();

  inline uint32_t GetLastError() const { return m_flags & kErrorCodeMask; }
  inline bool IsGood() const { return GetLastError() == Error::kSuccess; }
  inline bool IsPathInvalid() const { return GetLastError() == Error::kInvalidPath; }
  inline bool Exists() const { return GetLastError() == Error::kFileError; }
  inline bool IsBusy() const { return GetLastError() == Error::kBusy; }

  [[refl::override]]
  Error Write(const void* buffer, size_t length, size_t* numWritten);
  Error Read(void* buffer, size_t length, size_t* numRead);
  Error GetSize(size_t* size);
  Error Seek(size_t offset, int whence, size_t* pos);

  struct Info {
    time_t AccessTime;
    time_t ModifyTime;
    time_t CreateTime;
    size_t uiFileSize;
  };

  Error GetInfo(Info& aInfo);

 private:
  Error OpenImpl(const char* path, AccessMode accessMode, OpenMode openMode);
  Error ReadImpl(void* buffer, size_t length, size_t* numRead);
  Error WriteImpl(const void* buffer, size_t length, size_t* numWritten);

  Error GetSizeImpl(size_t* size);
  Error SeekImpl(size_t offset, int whence, size_t* pos);
  Error GetInfoImpl(Info&);

  inline Error SetLastError(Error aError) {
    m_flags &= kCacheMask;
    m_flags |= aError;
    return aError;
  }

 private:
  
  static constexpr uint32_t kErrorCodeMask = 0x3FFFFFFF;
  static constexpr uint32_t kCacheMask = 0x80000000;

  uint32_t m_flags = 1;
  union {
    void* m_pHandle;
    int32_t m_fileDescriptor;
  };
};

static_assert(sizeof(BSSystemFile) == 0x10);
} 

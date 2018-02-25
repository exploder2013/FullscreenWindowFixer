#include "Process.h"

bool Process::ErasePEHeader()
{
	PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER)Global::hModule;
	PIMAGE_NT_HEADERS nt_header = nullptr;

	if( dos_header )
	{
		if( dos_header->e_magic == IMAGE_DOS_SIGNATURE )
		{
			nt_header = (PIMAGE_NT_HEADERS)( (PBYTE)dos_header + dos_header->e_lfanew );
			if( nt_header )
			{
				if( nt_header->Signature == IMAGE_NT_SIGNATURE )
				{
					memset( (void*)dos_header, 0, nt_header->OptionalHeader.SizeOfHeaders );
					return true;
				}
			}
		}
	}

	return false;
}

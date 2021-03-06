
/***********************************************************************
	Easily port service

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, China ZheJiang HangZhou JianDe, Mail: powersuite@hotmaol.com

	@doc service cert document

	@module	srvcert.cc | service cert implement file

***********************************************************************/

/**********************************************************************
This program is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
LICENSE.GPL3 for more details.
***********************************************************************/

#include "srvcert.h"


cert_t* alloc_certs(int secu, const tchar_t* sz_path)
{
	cert_t* pcrt;
	link_t_ptr ilk, ptr_list;
	tchar_t sz_file[PATH_LEN];
	tchar_t sz_token[RES_LEN];
	int i;

	if (secu == _SECU_SSL)
	{
		pcrt = (cert_t*)xmem_alloc(sizeof(cert_t));

		xsprintf(sz_file, _T("%s/ssl/sslsrv.crt"), sz_path);
		pcrt->srv_crt = xshare_srv(SSL_SSLSRV_CRT, sz_file, X509_CERT_SIZE);

		xsprintf(sz_file, _T("%s/ssl/sslsrv.key"), sz_path);
		pcrt->srv_key = xshare_srv(SSL_SSLSRV_KEY, sz_file, RSA_KEY_SIZE);

		xsprintf(sz_file, _T("%s/ssl/ca/*.crt"), sz_path);
		ptr_list = create_list_doc();
		xfile_list(NULL, sz_file, ptr_list);

		pcrt->ca_count = get_list_child_item_count(ptr_list) + 1;
		pcrt->ca_chain = (xhand_t*)xmem_alloc(pcrt->ca_count * sizeof(xhand_t));

		i = 0;
		xsprintf(sz_file, _T("%s/ssl/sslca.crt"), sz_path);
		xsprintf(sz_token, _T("%s%d"), SSL_SSLCA_CRT, i);
		pcrt->ca_chain[i] = xshare_srv(sz_token, sz_file, X509_CERT_SIZE);
		if (pcrt->ca_chain[i])
		{
			i++;
		}

		ilk = get_list_first_child_item(ptr_list);
		while (ilk)
		{
			if (is_list_file_item(ilk))
			{
				xsprintf(sz_file, _T("%s/ssl/ca/%s"), sz_path, get_list_item_file_name_ptr(ilk));
				xsprintf(sz_token, _T("%s%d"), SSL_SSLCA_CRT, i);
				pcrt->ca_chain[i] = xshare_srv(sz_token, sz_file, X509_CERT_SIZE);
				if (pcrt->ca_chain[i])
				{
					i++;
				}
			}

			ilk = get_list_next_sibling_item(ilk);
		}
		destroy_list_doc(ptr_list);

		return pcrt;
	}
	else if (secu == _SECU_XSL)
	{
		pcrt = (cert_t*)xmem_alloc(sizeof(cert_t));

		xsprintf(sz_file, _T("%s/xsl/xslsrv.crt"), sz_path);
		pcrt->srv_crt = xshare_srv(XSL_XSLSRV_CRT, sz_file, X509_CERT_SIZE);

		xsprintf(sz_file, _T("%s/xsl/xslsrv.key"), sz_path);
		pcrt->srv_key = xshare_srv(XSL_XSLSRV_KEY, sz_file, RSA_KEY_SIZE);

		xsprintf(sz_file, _T("%s/xsl/ca/*.crt"), sz_path);
		ptr_list = create_list_doc();
		xfile_list(NULL, sz_file, ptr_list);

		pcrt->ca_count = get_list_child_item_count(ptr_list) + 1;
		pcrt->ca_chain = (xhand_t*)xmem_alloc(pcrt->ca_count * sizeof(xhand_t));

		i = 0;
		xsprintf(sz_file, _T("%s/xsl/xslca.crt"), sz_path);
		xsprintf(sz_token, _T("%s%d"), XSL_XSLCA_CRT, i);
		pcrt->ca_chain[i] = xshare_srv(sz_token, sz_file, X509_CERT_SIZE);
		if (pcrt->ca_chain[i])
		{
			i++;
		}

		ilk = get_list_first_child_item(ptr_list);
		while (ilk)
		{
			if (is_list_file_item(ilk))
			{
				xsprintf(sz_file, _T("%s/xsl/ca/%s"), sz_path, get_list_item_file_name_ptr(ilk));
				xsprintf(sz_token, _T("%s%d"), XSL_XSLCA_CRT, i);
				pcrt->ca_chain[i] = xshare_srv(sz_token, sz_file, X509_CERT_SIZE);
				if (pcrt->ca_chain[i])
				{
					i++;
				}
			}

			ilk = get_list_next_sibling_item(ilk);
		}
		destroy_list_doc(ptr_list);

		return pcrt;
	}
	else
	{
		return NULL;
	}
}

void free_certs(cert_t* pcrt)
{
	int i;

	if (pcrt->srv_crt)
	{
		xshare_close(pcrt->srv_crt);
		pcrt->srv_crt = NULL;
	}

	if (pcrt->srv_key)
	{
		xshare_close(pcrt->srv_key);
		pcrt->srv_key = NULL;
	}

	for (i = 0; i < pcrt->ca_count; i++)
	{
		if (pcrt->ca_chain[i])
		{
			xshare_close(pcrt->ca_chain[i]);
		}
	}
	xmem_free(pcrt->ca_chain);
	pcrt->ca_chain = NULL;
	pcrt->ca_count = 0;
}

void set_certs(int secu, xhand_t bio)
{
	xhand_t bh;
	byte_t* buf;
	int i, len;
	tchar_t sz_cert[RES_LEN];

	if (secu == _SECU_SSL)
	{
		bh = xshare_cli(SSL_SSLSRV_CRT, X509_CERT_SIZE);
		if (bh)
		{
			buf = (byte_t*)xshare_lock(bh, 0, X509_CERT_SIZE);
			if (buf)
			{
				len = a_xslen((schar_t*)buf);
				if (len)
				{
					xssl_set_cert(bio, buf, len);
				}
				xshare_unlock(bh, 0, X509_CERT_SIZE, buf);
			}
			xshare_close(bh);
		}

		bh = xshare_cli(SSL_SSLSRV_KEY, RSA_KEY_SIZE);
		if (bh)
		{
			buf = (byte_t*)xshare_lock(bh, 0, RSA_KEY_SIZE);
			if (buf)
			{
				len = a_xslen((schar_t*)buf);
				if (len)
				{
					xssl_set_rsa(bio, buf, len, NULL, 0);
				}
				xshare_unlock(bh, 0, RSA_KEY_SIZE, buf);
			}
			xshare_close(bh);
		}

		i = 0;
		do
		{
			xsprintf(sz_cert, _T("%s%d"), SSL_SSLCA_CRT, i);
			bh = xshare_cli(sz_cert, X509_CERT_SIZE);
			if (!bh)
				break;

			buf = (byte_t*)xshare_lock(bh, 0, X509_CERT_SIZE);
			if (buf)
			{
				len = a_xslen((schar_t*)buf);
				if (len)
				{
					if (!xssl_set_ca(bio, buf, len))
						len = 0;
				}
				xshare_unlock(bh, 0, X509_CERT_SIZE, buf);
			}
			xshare_close(bh);

			if (!buf || !len)
				break;
		} while (++i);
	}
	else if (secu == _SECU_XSL)
	{
		bh = xshare_cli(XSL_XSLSRV_CRT, X509_CERT_SIZE);
		if (bh)
		{
			buf = (byte_t*)xshare_lock(bh, 0, X509_CERT_SIZE);
			if (buf)
			{
				len = a_xslen((schar_t*)buf);
				if (len)
				{
					xxsl_set_cert(bio, buf, len);
				}
				xshare_unlock(bh, 0, X509_CERT_SIZE, buf);
			}
			xshare_close(bh);
		}

		bh = xshare_cli(XSL_XSLSRV_KEY, RSA_KEY_SIZE);
		if (bh)
		{
			buf = (byte_t*)xshare_lock(bh, 0, RSA_KEY_SIZE);
			if (buf)
			{
				len = a_xslen((schar_t*)buf);
				if (len)
				{
					xxsl_set_rsa(bio, buf, len, NULL, 0);
				}
				xshare_unlock(bh, 0, RSA_KEY_SIZE, buf);
			}
			xshare_close(bh);
		}

		i = 0;
		do
		{
			xsprintf(sz_cert, _T("%s%d"), XSL_XSLCA_CRT, i);
			bh = xshare_cli(sz_cert, X509_CERT_SIZE);
			if (!bh)
				break;

			buf = (byte_t*)xshare_lock(bh, 0, X509_CERT_SIZE);
			if (buf)
			{
				len = a_xslen((schar_t*)buf);
				if (len)
				{
					if (!xxsl_set_ca(bio, buf, len))
						len = 0;
				}
				xshare_unlock(bh, 0, X509_CERT_SIZE, buf);
			}
			xshare_close(bh);

			if (!buf || !len)
				break;
		} while (++i);
	}
}
/*
 *  param.h
 *  wxMedia
 *
 *  Created by Casey Tucker on 2010-09-13.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */
#pragma once

#include "myparams.h"
#include "wx/fileconf.h"
#define PN(s) *((int*)s)

typedef union
{
	unsigned int v;
	unsigned char c[4];
} ParamName;

class Param
{
public:
	fl *value;
	int *integer;
	Param *parent;
	void (*Callback)(fl);
	fl (*func)(fl);
	fl (*func1)(fl);
	ParamName name;
	//Param(int n){ name.v = n; } 
	//Param(char *n){ for(int i=0; i < 4; i++) name.c[i] = n[i]; }
	//Param(ParamName n){ name.v = n.v; }
	Param()
	{
		integer = 0;
		value = 0;
		func = NULL; func1 = NULL;
	}
	Param( fl *v, const char *c=0, void (*f)(fl) =0 )
	{
		integer = 0;
		value = v;
		if( c != 0 ) SetParamName( c );
		if( f != 0 ) Callback = f;
		func = NULL; func1 = NULL;
	}
	Param( int *i, const char *c=0, void (*f)(fl) =0 )
	{
		integer = i;
		value = 0;
		if( c != 0 ) SetParamName( c );
		if( f != 0 ) Callback = f;
		func = NULL; func1 = NULL;
	}
	void SetParamName( ParamName n )
	{
		name.v = n.v;
	}
	void SetParamName( const char *c )
	{
		for(int i=0; i < 4; i++)
			name.c[i] = c[i];
	}
	fl GetValue()
	{
		if( value )   return *value;
		if( integer ) return *integer;
		return -1.0f;
	}
	void SetValue(fl v)
	{
		if( value ) *value = v;
		if( integer ) *integer = v;
	}
};

#include <iostream>
class Set: public Param
{
public:
	int scount;
	int pcount;
	Param *param[256];
	Set *set[256];
	
	Set() : Param() { scount = 0; pcount = 0; }
	
	
	void AddSet(Set *s, const char *c=0 )
	{
		if( c != 0 ) s->SetParamName( c );
		set[scount++] = s;
	}
	virtual void AddParam( fl *v, const char *c=0, void (*f)(fl)=0 )
	{
		Param *p = new Param(v, c, f );
		p->parent = this;
		if( c != 0 ) p->SetParamName( c );
		param[pcount++] = p;
	}
	virtual void AddParam( int *i, const char *c=0, void (*f)(fl)=0 )
	{
		Param *p = new Param(i, c, f );
		p->parent = this;
		if( c != 0 ) p->SetParamName( c );
		param[pcount++] = p;

	}
	virtual void SetParam( int n, fl v )
	{
		//SetParam( n, v );
	}
	
	virtual fl GetParam( int n, fl v )
	{
		for(int i=0; i < pcount; i++)   // hey, remind me to replace this with 
		{
			if( n == param[i]->name.v )
				return *(param[i]->value);
		}
		return -1;
	}

	virtual Param * FindParam( int n )
	{
		for(int i=0; i < pcount; i++)   // hey, remind me to replace this with 
		{
			if( n == param[i]->name.v )
				return param[i];
		}
		return 0;
	}
	
	virtual Set * GetInto( int n )
	{
		for( int i=0; i < scount; i++ ) // a hash table for better optimization
		{
			if( n == set[i]->name.v )
				return set[i];
		}
		return this;
	}
	virtual void Save()
	{
		wxConfigBase *config = wxConfigBase::Get();
		for(int s=0; s < scount; s++)
		{
			wxString path;
			for(int i=0; i < 4 and set[s]->name.c[i] !=0; i++)
				path << (wxChar)(set[s]->name.c[i]);
			
			//std::cout << path.mb_str() << "\n";

			config->SetPath( path );
			set[s]->Save();
			config->SetPath( _T("..") );
		}
		for(int p=0; p < pcount; p++)
		{
			wxString parm;
			for(int i=0; i < 4 and param[p]->name.c[i] !=0; i++)
				parm << (wxChar)(param[p]->name.c[i]);
			
			//std::cout << "\t" << parm.mb_str() << "\t" << param[p]->GetValue() << "\n";

			config->Write( parm, param[p]->GetValue() );
		}
	}
	virtual void Load()
	{
		wxConfigBase *config = wxConfigBase::Get();
		for(int s=0; s < scount; s++)
		{
			wxString path;
			for(int i=0; i < 4 and set[s]->name.c[i] !=0; i++)
				path << (wxChar)(set[s]->name.c[i]);
			
			//std::cout << path.mb_str() << "\n";

			config->SetPath( path );
			set[s]->Load();
			config->SetPath( _T("..") );
		}
		for(int p=0; p < pcount; p++)
		{
			wxString parm;
			double v;

			for(int i=0; i < 4 and param[p]->name.c[i] !=0; i++)
				parm << (wxChar)(param[p]->name.c[i]);
			
			if( config->Read( parm, &v ) )
			{
				//param[p]->SetValue( v );
				SetParam( param[p]->name.v , v );
			}

			//std::cout << "\t" << parm.mb_str() << "\t" << param[p]->GetValue() << "\n";
		}
	}

};

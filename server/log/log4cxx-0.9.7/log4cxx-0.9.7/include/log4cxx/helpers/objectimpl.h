/*
 * Copyright 2003,2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#ifndef _LOG4CXX_HELPERS_OBJECT_IMPL_H
#define _LOG4CXX_HELPERS_OBJECT_IMPL_H

#include <log4cxx/config.h>
#include <log4cxx/helpers/object.h>
#include <log4cxx/helpers/criticalsection.h>

namespace log4cxx
{
	namespace helpers
	{
		/** Implementation class for Object.*/
		class LOG4CXX_EXPORT ObjectImpl : public virtual Object
		{
		public:
			ObjectImpl();
			virtual ~ObjectImpl();
			void addRef() const;
			void releaseRef() const;
			virtual void lock() const;
			virtual void unlock() const;
			virtual void wait() const;
			virtual void notify() const;
			virtual void notifyAll() const;

		protected:
			mutable long volatile ref;
			mutable CriticalSection cs;
			mutable void * eventList;
		};
	} 
} 

#endif //_LOG4CXX_HELPERS_OBJECT_IMPL_H
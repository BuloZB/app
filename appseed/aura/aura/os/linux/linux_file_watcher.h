/**
	Implementation header file for Linux based on inotify.

	@author James Wynn
	@date 4/15/2009

	Copyright (c) 2009 James Wynn (james@jameswynn.com)

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
*/
#pragma once


namespace file_watcher
{
	/// Implementation for Linux based on inotify.
	/// @class os_file_watcher
	class os_file_watcher : public file_watcher_impl
	{
	public:
		/// type for a map from id to watch_struct pointer
		typedef map<file_watch_id, file_watch_id, watch_struct *, watch_struct*> WatchMap;

	public:
		///
		///
		os_file_watcher(::aura::application * papp);

		///
		///
		virtual ~os_file_watcher();

		/// Add a directory watch
		/// @exception file_not_found_exception Thrown when the requested directory does not exist
		file_watch_id add_watch(const string & directory, file_watch_listener* watcher, bool bRecursive, bool bOwn);

		/// Remove a directory watch. This is a brute force lazy search O(nlogn).
		void remove_watch(const string & directory);

		/// Remove a directory watch. This is a map lookup O(logn).
		void remove_watch(file_watch_id watchid);

		string watch_path(file_watch_id watchid);

		/// Updates the watcher. Must be called often.
		void update();

		/// Handles the action
		void handle_action(action * paction);

	private:
		/// Map of id to watch_struct pointers
		WatchMap m_watchmap;
		/// The last watchid
		file_watch_id mLastWatchID;
		/// inotify file descriptor
		int32_t mFD;
		/// time out data
		struct timeval mTimeOut;
		/// File descriptor set
		fd_set mDescriptorSet;


	};


} // namespace file_watcher


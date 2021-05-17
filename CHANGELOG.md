## 17.06.2021
* Syntax highlighting (:D), shorten list page substantially
* Prettify list page to not look like garbage (tf was I thinking?)
* Improve rate limit handling
* Remove useless functions

## 15.05.2021
* Error handling for API requests
* Buffer management for issues and lists
* Set up mapping for showing issue and comments

## 14.05.2021
* Add optional URL parameter to list function
* Fix markdown edge-cases causing broken syntax highlighting

## 11.05.2021
* Finalize token management

## 10.05.2021
* Add command to set the URL
* Add command to auto-detect URL
* Start documentation
    * Including documentation on GH rate limiting, as well as an effective bypass of said rate limiting (bypassing at this scale: instead of 60 requests per hour, we get 5000 per hour when authenticated)
* Set up ListIssues(), DetectUrl(), and more communication
